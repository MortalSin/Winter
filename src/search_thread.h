/*
 *  Winter is a UCI chess engine.
 *
 *  Copyright (C) 2016 Jonas Kuratli, Jonathan Maurer, Jonathan Rosenthal
 *  Copyright (C) 2017-2018 Jonathan Rosenthal
 *
 *  Winter is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Winter is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/*
 * search_thread.h
 *
 *  Created on: Nov 3, 2018
 *      Author: Jonathan Rosenthal
 */

#ifndef SRC_SEARCH_THREAD_H_
#define SRC_SEARCH_THREAD_H_

#include "board.h"
#include "general/types.h"
#include "general/settings.h"
#include <array>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace search {

struct Thread {
  Thread();

  void clear_killers_and_counter_moves() {
    static_scores.fill(kNoScore);
    for (size_t i = 0; i < killers.size(); i++) {
      killers[i][0] = 0;
      killers[i][1] = 0;
    }

    for (size_t c = 0; c < 2; c++) {
      for (PieceType pt = kPawn; pt <= kKing; pt++) {
        for (Square sq = 0; sq < 64; sq++) {
          counter_moves[c][pt][sq] = kNullMove;
        }
      }
    }

    for (PieceType pt = kPawn; pt <= kKing; pt++) {
      for (Square sq = 0; sq < 64; sq++) {
        for (PieceType pt_i = kPawn; pt_i <= kKing; pt_i++) {
          for (Square sq_i = 0; sq_i < 64; sq_i++) {
            counter_move_history[pt][sq][pt_i][sq_i] = 0;
          }
        }
      }
    }

  }

  void search();
  //It may make sense to perturb move ordering slightly for LazySMP helper threads.
  void perturb_root_moves();

  // The following functions return the trend of the score. Regular and strict effect the handling of
  // kNoMove scores from the previous position.
  bool improving() const;
  bool strict_improving() const;
  bool worsening() const;
  bool strict_worsening() const;

  void set_static_score(const Score score) {
    assert(board.get_num_made_moves() >= root_height);
    assert(score == kNoScore || (score >= kMinScore && score <= kMaxScore));
    Depth height = std::min((Depth)board.get_num_made_moves() - root_height, settings::kMaxDepth - 1);
    static_scores[height] = score;
  }

  int32_t get_cmh_score(const PieceType opp_piecetype, const Square opp_des,
                        const PieceType piecetype, const Square des) const {
    return counter_move_history[opp_piecetype][opp_des][piecetype][des];
  }

  void update_cmh_scores(const PieceType opp_piecetype, const Square opp_des,
                         const PieceType piecetype, const Square des, const int32_t score) {

    counter_move_history[opp_piecetype][opp_des][piecetype][des] += 32 * score
        - counter_move_history[opp_piecetype][opp_des][piecetype][des]
                      * std::abs(score) / 512;
  }

  //Multithreading objects
  int id;

  //Data for search local to the thread
  Board board;
  std::vector<Move> moves;
  Depth current_depth;
  Array2d<Move, 1024, 2> killers;
  Array3d<Move, 2, 6, 64> counter_moves;
  Array2d<Array2d<int32_t, 6, 64>, 6, 64> counter_move_history;
  Depth root_height;
  std::array<Score, settings::kMaxDepth> static_scores;
  std::atomic<size_t> nodes;
  std::atomic<size_t> max_depth;
};

struct ThreadPool {
  ThreadPool();
  //Set number of threads including main thread
  void set_num_threads(size_t num_threads);
  void clear_killers_and_countermoves() {
    main_thread->clear_killers_and_counter_moves();
    for (Thread* thread : helpers) {
      thread->clear_killers_and_counter_moves();
    }
  }
  size_t count() {
    return helpers.size() + 1;
  }
  size_t node_count() {
    size_t sum = main_thread->nodes;
    for (Thread* helper : helpers) {
      sum += helper->nodes;
    }
    return sum;
  }

  size_t max_depth() {
    size_t max_d = main_thread->max_depth;
    for (Thread* helper : helpers) {
      if (helper->max_depth > max_d) {
        max_d = helper->max_depth;
      }
    }
    return max_d;
  }

  void reset_node_count() {
    main_thread->nodes = 0;
    for (Thread* helper : helpers) {
      helper->nodes = 0;
    }
  }

  bool ignorance_smp;
  std::atomic_bool end_search;
  std::vector<Thread*> helpers;
  Thread* main_thread;
};

//The only instance of Threads
extern ThreadPool Threads;

}

#endif /* SRC_SEARCH_THREAD_H_ */
