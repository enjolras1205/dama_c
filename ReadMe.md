## 模板使用
### 开发者文档：https://alidocs.dingtalk.com/i/nodes/P7QG4Yx2Jpx4OolYCZqRam13J9dEq3XD?doc_type=wiki_doc
### 传输内容
1. query传输的数据包括
   * board 棋盘信息，和棋谱上的位置对应
   * winners  是否已经有赢家
   * move detail 对手的上一步的move是什么（第一次白棋下子没有）
   * king 这个回合是否有棋子变成王（第一次白棋下子没有）
   * code 具体code代表的含义请看开发者文档
2. move返回的数据包括
   * board 棋盘信息，和棋谱上的位置对应
   * move detail 对手的上一步的move是什么
   * king 这个回合是否有棋子变成王       
   * winners  是否已经有赢家        
   * code 具体code代表的含义请看开发者文档

### 需要写回的信息
调用sendMovesRequest，里面已经携带好了token，需要把moves传进去
### 模板代码里各个函数的含义

1. `getNextMove(board)` 这里是计算具体移动，开发者自行替换自己的代码
2. `convertToMatrix(data, size = 8)` 这个方法非必要，是用来吧传输进来的棋盘转化为二维矩阵方便开发者使用，开发者可换别的表现形式
3. `parseResponse` 这里把json数据解析
4. `convertIndexIntoInteger`配合`convertToMatrix(data, size = 8)`使用
5. `convertIntegerIntoString`配合`convertToMatrix(data, size = 8)`使用
6. `sendInitRequest` 必要，发起Query请求，具体看开发者文档
7. `sendMovesRequest`必要，发起Moves请求，具体看开发者文档
