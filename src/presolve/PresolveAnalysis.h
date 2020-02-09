/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                       */
/*    This file is part of the HiGHS linear optimization suite           */
/*                                                                       */
/*    Written and engineered 2008-2020 at the University of Edinburgh    */
/*                                                                       */
/*    Available as open-source under the MIT License                     */
/*                                                                       */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**@file presolve/PresolveAnalysis.h
 * @brief
 * @author Julian Hall, Ivet Galabova, Qi Huangfu and Michael Feldmeier
 */
#ifndef PRESOLVE_PRESOLVE_ANALYSIS_H_
#define PRESOLVE_PRESOLVE_ANALYSIS_H_

#include <string>
#include <vector>

#include "util/HighsTimer.h"

enum PresolveRule {
  // Presolve rules.
  EMPTY_ROW,
  FIXED_COL,
  SING_ROW,
  DOUBLETON_EQUATION,
  FORCING_ROW,
  REDUNDANT_ROW,
  DOMINATED_ROW_BOUNDS,
  FREE_SING_COL,
  SING_COL_DOUBLETON_INEQ,
  IMPLIED_FREE_SING_COL,
  DOMINATED_COLS,
  WEAKLY_DOMINATED_COLS,
  DOMINATED_COL_BOUNDS,
  EMPTY_COL,
  // HTICK_PRE_DUPLICATE_ROWS,
  // HTICK_PRE_DUPLICATE_COLUMNS,

  // Number of presolve rules.
  PRESOLVE_RULES_COUNT,

  // Items required by postsolve
  DOUBLETON_EQUATION_ROW_BOUNDS_UPDATE,
  DOUBLETON_EQUATION_X_ZERO_INITIALLY,
  DOUBLETON_EQUATION_NEW_X_NONZERO,
  DOUBLETON_EQUATION_NEW_X_ZERO_AR_UPDATE,
  DOUBLETON_EQUATION_NEW_X_ZERO_A_UPDATE,
  SING_COL_DOUBLETON_INEQ_SECOND_SING_COL,
  FORCING_ROW_VARIABLE
};

struct PresolveRuleInfo {
  PresolveRuleInfo(PresolveRule id, std::string name, std::string name_ch3)
      : rule_id(id),
        rule_name(std::move(name)),
        rule_name_ch3(std::move(name_ch3)) {}
  PresolveRule rule_id;

  std::string rule_name;
  std::string rule_name_ch3;

  int attempts = 0;
  int count_applied = 0;
  int rows_removed = 0;
  int cols_removed = 0;

  int clock_id = 0;
  double total_time = 0;
};

void initializePresolveRuleInfo(std::vector<PresolveRuleInfo>& rules);

class PresolveTimer {
 public:
  PresolveTimer(HighsTimer& timer) : timer_(timer) {
    initializePresolveRuleInfo(rules_);
    for (PresolveRuleInfo rule : rules_) {
      int clock_id =
          timer_.clock_def(rule.rule_name.c_str(), rule.rule_name_ch3.c_str());
      rule.clock_id = clock_id;
    }
  }

  void recordStart(PresolveRule rule) { timer_.start(rules_[rule].clock_id); }

  void recordFinish(PresolveRule rule) { timer_.stop(rules_[rule].clock_id); }

 private:
  HighsTimer& timer_;
  std::vector<PresolveRuleInfo> rules_;
};

#endif