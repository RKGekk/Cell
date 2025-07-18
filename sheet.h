#pragma once

#include <functional>
#include <vector>

#include "cell.h"
#include "common.h"

class Sheet : public SheetInterface {
public:
    using SheetList = std::unordered_map<Position, std::unique_ptr<Cell>, HashPosition>;

    Sheet();

    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

private:
    void CheckPosInPlace(Position pos) const;
    void MakeCell(Position pos, std::string text);
    Size CreatePrintableSize() const;

    template <typename Func>
    void Printer(std::ostream& output, Func func) const {
        Size sz = GetPrintableSize();
        for (int r = 0; r < sz.rows; ++r) {
            bool is_first = true;
            for (int c = 0; c < sz.cols; ++c) {
                if (!is_first) {
                    output << '\t';
                }
                is_first = false;
                if (const auto* ptr_value = GetCell({ r, c })) {
                    func(ptr_value);
                }
            }
            output << '\n';
        }
    }

    SheetList sheet_list_;
    Positions positions_;
};

// Создаёт готовую к работе пустую таблицу.
std::unique_ptr<SheetInterface> CreateSheet();
