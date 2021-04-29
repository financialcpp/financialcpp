const { gql } = require('apollo-server')

// A schema is a collection of type definitions (hence "typeDefs")
// that together define the "shape" of queries that are executed against
// your data.
export const typeDefs = gql`
  type Symbol {
    name: String
  }


  type Tick {
      time: String
      price: Float
      size: Int
  }

#   type Bid {
#     size: Int
#     price: Int
#   }

#   type Ask {
#     size: Int
#     price: Int
#   }

#   type Quote {
#       time: Int
#       bid: Bid
#       ask: Ask
#       condition: Int
#   }

#   type Bar {
#     open: Float
#     high: Float
#     low: Float
#     close: Float
#     volume: Int
#   }

  type SymbolSet {
      name: String
      description: String
  }

#   enum Unit {
#     MILLISECOND
#     SECOND
#     MINUTE
#     HOUR
#     DAY
#     WEEK
#     YEAR
#   }

#   enum HistoryType {
#       TICK
#       QUOTE
#   }

#   type Backtest {
#       id: Int
#       complete: Boolean
#       progress: Int
#       start: Int
#       end: Int
#       fromDate: Int
#       toDate: Int
#   }

#   type Study {
#       id: Int
#       complete: Boolean
#       progress: Int
#       start: Int
#       end: Int
#       fromDate: Int
#       toDate: Int
#   }

#   type Server {
#       hostname: String
#   }

#   type Provider {
#       name: String
#       description: String
#   }

  type Query {
      tickHistory(symbol: String, provider: String, fromDate: String, toDate: String): [Tick!]!
    #   quoteHistory(provider: String, fromDate: String, toDate: String): [Quote!]!
      barHistory(symbol: String, provider: String,  sample: Int, unit: Unit): [Bar!]!

    #   backtests(completed: Boolean): [Backtest!]!
    #   studies: [Study!]!
    #   servers: [Server!]!
      
      symbols(set: String): [Symbol!]!

    #   symbolSets: [SymbolSet!]!
  }


#   type Error {
#       code: String
#       prettyMessage: String
#   }

#   type StrategyOptions {
#       name: String
#       options: String
#   }

#   type Mutation {
#       startBacktest(fromDate: String, toDate: String, symbols: SymbolSet, strategies: [StrategyOptions!]!, provider: String): BacktestSubmitResult
#       cancelBacktest(id: Int): Status
#   }
`
