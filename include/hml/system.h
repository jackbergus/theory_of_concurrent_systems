
/*
 * system.h
 * This file is part of hml
 *
 * Copyright (C) 2022 - Giacomo Bergami
 *
 * hml is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * hml is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with hml. If not, see <http://www.gnu.org/licenses/>.
 */

//
// Created by giacomo on 14/02/2022.
//

#ifndef HML_SYSTEM_H
#define HML_SYSTEM_H

#include <hml/hml_formula.h>

enum Sol {
    Sol_MIN,
    Sol_MAX
};

using System = std::pair<Sol, hml_formula>;

System inv(size_t i, const hml_formula& f);
System pos(size_t i, const hml_formula& f);
System even(size_t i, const hml_formula& f);
System safe(size_t i, const hml_formula& f);

#endif //HML_SYSTEM_H
