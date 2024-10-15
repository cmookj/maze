#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

using maze_t = vector<vector<int>>;

maze_t maze = {
    {0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 1, 0, 0},
    {0, 1, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0},
};

struct pos_t {
  int i, j;

  pos_t (int _i, int _j)
      : i{_i}
      , j{_j} {}

  bool
  operator== (const pos_t& rhs) {
    return i == rhs.i && j == rhs.j;
  }

  bool
  operator!= (const pos_t& rhs) {
    return !(this->operator== (rhs));
  }
};

// This function is necessary to use the ostream_iterator<> for standard output.
ostream&
operator<< (ostream& os, const pos_t& p) {
  return os << p.i << ", " << p.j;
}

// -----------------------------------------------------------------------------
// backtrack_t
// -----------------------------------------------------------------------------
using backtrack_t = vector<vector<pos_t>>;

// Each cell contains the coordinate of the previous cell, which is the parent
// node if the breadth-first search is considered as a tree.  When the search
// algorithm arrives at the goal cell, we can reconstruct the route from the
// initial position by backtracking.
backtrack_t
create_backtrack (const maze_t& mz) {
  backtrack_t backtrack;

  int height = mz.size();
  int width  = mz.begin()->size();

  for (int j = 0; j < height; ++j) {
    backtrack.push_back (vector<pos_t> (width, pos_t (0, 0)));
  }

  return backtrack;
}

void
record_backtrack (backtrack_t& backtrack, const pos_t& p, const pos_t& prev) {
  backtrack[p.i][p.j] = prev;
}

// -----------------------------------------------------------------------------
// visited_map_t
// -----------------------------------------------------------------------------
using visited_map_t = vector<vector<bool>>;

// When a cell was visited, set it true.
// Otherwise, false.
visited_map_t
create_visited_map (const maze_t& mz) {
  visited_map_t visited_map;

  int height = mz.size();
  int width  = mz.begin()->size();

  for (int j = 0; j < height; ++j) {
    visited_map.push_back (vector<bool> (width, false));
  }

  return visited_map;
}

bool
arrived (const deque<pos_t>& search_queue, const pos_t& goal) {
  auto iter = find_if (search_queue.begin(), search_queue.end(), [&] (const auto& p) {
    return p.i == goal.i && p.j == goal.j;
  });

  if (iter == search_queue.end()) return false;

  return true;
}

bool
should_visit (const pos_t& p, const maze_t& mz, const visited_map_t& visited_map) {
  if (mz[p.i][p.j] == 0) return false;
  return !visited_map[p.i][p.j];
}

void
insert_children (
    deque<pos_t>&  search_queue,
    const maze_t&  mz,
    visited_map_t& visited_map,
    backtrack_t&   backtrack,
    const pos_t&   current
) {
  vector<pos_t> candidates;
  candidates.push_back (pos_t (current.i - 1, current.j));  // up
  candidates.push_back (pos_t (current.i, current.j - 1));  // left
  candidates.push_back (pos_t (current.i, current.j + 1));  // right
  candidates.push_back (pos_t (current.i + 1, current.j));  // down

  // Add the 4 adjacent cells if they are free and have not visited yet.
  for (const auto& p : candidates) {
    if (should_visit (p, mz, visited_map)) {
      search_queue.push_back (p);
      record_backtrack (backtrack, p, current);
    }
  }
}

vector<pos_t>
path_to (const pos_t& start, const pos_t& goal, const maze_t& mz) {
  auto visited_map = create_visited_map (mz);
  auto backtrack   = create_backtrack (mz);

  deque<pos_t> search_queue;

  search_queue.push_back (start);

  // Breadth-first search
  while (!arrived (search_queue, goal)) {
    // Since we have not arrived at the goal, we are going to add all the
    // adjacent cells (children) which have not visited.  After adding the
    // children, we remove the current cells from the search queue.
    int count_current_children = search_queue.size();
    for (int i = 0; i < count_current_children; ++i) {
      auto& current                     = search_queue[i];
      visited_map[current.i][current.j] = true;
      insert_children (search_queue, mz, visited_map, backtrack, current);
    }
    for (int i = 0; i < count_current_children; ++i) {
      search_queue.pop_front();
    }
  }

  // Build-up backtrack
  pos_t         current = goal;
  vector<pos_t> route;

  while (current != start) {
    route.push_back (current);
    current = backtrack[current.i][current.j];
  }
  route.push_back (start);

  return route;
}

int
main (int argc, char* argv[]) {
  std::cout << "Hello, Maze!\n";
  auto path = path_to (pos_t (1, 1), pos_t (5, 5), maze);

  copy (path.begin(), path.end(), ostream_iterator<pos_t> (cout, "\n"));

  return 0;
}
