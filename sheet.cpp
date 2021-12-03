#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

#include "sheet.h"
#include "cell.h"
#include "common.h"

using namespace std::literals;

Sheet::Sheet() {
}

Sheet::~Sheet() {
}

void Sheet::SetCell(Position pos, std::string text) {
    CheckPosInPlace(pos);
    if (!text.empty()) {
        positions_.insert(pos);
    }
    if (sheet_list_.count(pos) == 0) {
        MakeCell(pos, std::move(text));
    }
    else {
        sheet_list_[pos]->Set(std::move(text));
    }
}

const CellInterface* Sheet::GetCell(Position pos) const {
    return const_cast<Sheet*>(this)->GetCell(pos);
}
CellInterface* Sheet::GetCell(Position pos) {
    CheckPosInPlace(pos);
    if (sheet_list_.count(pos) == 0) {
        return nullptr;
    }
    Cell* cell = sheet_list_.at(pos).get();
    if (!cell || (cell->IsEmpty() && !cell->IsReferenced())) {
        return nullptr;
    }
    return cell;
}

void Sheet::ClearCell(Position pos) {
    SetCell(pos, std::string());
    positions_.erase(pos);
}

Size Sheet::GetPrintableSize() const {
    int max_r = 0;
    int max_c = 0;
    for (Position pos : positions_) {
        max_r = std::max(max_r, pos.row + 1);
        max_c = std::max(max_c, pos.col + 1);
    }
    return { max_r, max_c };
}

void Sheet::PrintValues(std::ostream& output) const {
    auto print_get_value = [&output](const CellInterface* ptr_value) {
        std::visit([&](const auto& x) { output << x; }, ptr_value->GetValue());
    };
    Printer(output, print_get_value);
}
void Sheet::PrintTexts(std::ostream& output) const {
    auto print_get_value = [&output](const CellInterface* ptr_value) {
        std::visit([&](const auto& x) { output << x; }, ptr_value->GetValue());
    };
    Printer(output, print_get_value);
}

void Sheet::CheckPosInPlace(Position pos) const {
    if (!pos.IsValid()) {
        using namespace std;
        std::stringstream ss;
        ss << "Position is invalid "s << pos.ToString();
        throw InvalidPositionException(ss.str());
    }
}

Size Sheet::CreatePrintableSize() const {
    int max_r = 0;
    int max_c = 0;
    for (Position pos : positions_) {
        max_r = std::max(max_r, pos.row + 1);
        max_c = std::max(max_c, pos.col + 1);
    }
    return { max_r, max_c };
}

void Sheet::MakeCell(Position pos, std::string text) {
    Cell* cell = new Cell(*this, pos);
    cell->Set(std::move(text));
    sheet_list_[pos].reset(cell);
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}
