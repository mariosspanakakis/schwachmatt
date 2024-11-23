# Universal Chess Interface (UCI)

## Basic Communication Flow

| **GUI**           | **ENGINE**                | **Meaning** |
| :---              | ---:                      | :--- |
| ```uci```         |                           | entry point |
|                   | ```id name <name>```      | specify engine name |
|                   | ```id author <author>```  | specify engine author |
|                   | ```uciok```               | state protocol compliance |
| ```ucinewgame```  |                           | let the engine initialize a new game |
| ```isready```     |                           | wait for the engine to be ready |
|                   | ```readyok```             | acknowledge readyness |
| ```position```    |                           | set up a new position on the board |
| ```go```          |                           | start searching the best move in the given position |
| ```stop```        |                           | stop searching as soon as possible |
|                   | ```bestmove```            | send the best move that has been found, must be sent for each ```go``` command |

## Notes

### ```isready``` and ```readyok```

```isready``` is sent by the GUI to synchronize with the engine, e.g. if the engine needs some time to complete a task.

> While thinking, the engine must be able to respond ```readyok``` while still thinking!

### ```go```

The ```go``` command is used to start a search on a position that has been set up before. This command has various parameters that specify the search depth and duration.