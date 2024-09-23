   
   #ifndef DFS_H
   #define DFS_H

   #include <vector>
   using Board = std::vector<std::vector<int>>;

   std::vector<std::vector<std::pair<int, int>>> move_list(Board &board, bool white);

   #endif // DFS_H
