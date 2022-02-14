/*
 * semantics.h
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

#include <hml/hml_formula.h>
#include <hml/LTS.h>
#include <hml/system.h>

#ifndef HML_SEMANTICS_H
#define HML_SEMANTICS_H

/**
 * Implementation of the HennessyMilner Logic's semantics
 * @param s
 * @param qar
 * @param repo
 * @param i
 * @param qset
 * @return
 */
ResultSet semantics(const hml_formula& s,
                    const LTS& qar,
                    std::vector<ResultSet>& repo,
                    size_t i,
                    const ResultSet& qset);


std::vector<ResultSet> solve_system(const std::vector<System>& next, const LTS& qar, size_t i = 0);

#endif //HML_SEMANTICS_H
