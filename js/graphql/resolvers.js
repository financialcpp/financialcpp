const fs = require('fs')

// const Financialcpp = require('../lib/index.js')
// const fpp = new Financialcpp()


const fakeTicks = () => {
    let ticks = []
    let time = new Date().getTime()
    for (let i = 0; i < 100; i++) {
        let price = Math.random() * 10 // create number from 0 to 10
        time = time + 1
        let tick = {
            time,
            price: price,
            size: Math.round(Math.random() * 1000), // size will be between 0 to 1000
        }
        ticks.push(tick)
    }
    return ticks
}

export const resolvers = {
    Query: {
        tickHistory: (parent, args, context, info) => {
            if (args.provider == 'fake-provider') {
                let ticks = fakeTicks()
                console.log(ticks[0]);
                return ticks
            } else {
                // fpp.tickHistory({
                //     ...args,
                //     fromDate: new Date(args.fromDate).getTime(),
                //     toDate: new Date(args.toDate).getTime()
                // })
            }
        },
        //     quoteHistory: (parent, args, context, info) => {},
        //     barHistory: (parent, args, context, info) => {},

        symbols: (parent, args, context, info) => {
            console.log(args);
            if (args.set == 'All') {
                let results = []
                return new Promise((resolve, reject) => {
                    fs.createReadStream('data.csv')
                        .pipe(csv({ headers: false }))
                        .on('data', (data) => results.push({ name: data[0] }))
                        .on('end', () => {
                            resolve(results)
                        })
                    // [
                    //   { NAME: 'Daffy Duck', AGE: '24' },
                    //   { NAME: 'Bugs Bunny', AGE: '22' }
                    // ]
                });
                return csv
            } else if (args.set = 'Software/Tech') {
                return [
                    { name: 'FB ' },
                    { name: 'AAPL' },
                    { name: 'AMZN' },
                    { name: 'NFLX' },
                    { name: 'GOOG' }
                ]
            }
        },
        //     symbolSets: (parent, args, context, info) => {
        //         return [
        //             {
        //                 name: 'All',
        //                 description: 'All stocks on NYSE or NASDAQ exchange'
        //             },
        //             {
        //                 name: 'Software/Tech',
        //                 description: 'FAANG'
        //             }
        //         ]
        //     },



        //     tickerSymbols: (parent, args, context, info) => {
        //         console.log(fpp.tickerSymbols());
        //       return fpp.tickerSymbols()
        //     },
        //     tick: (parent, args) => {
        //       let { tickerSymbol, bin = 1, fromDate, toDate, provider } = args
        //       return fpp.tick({ tickerSymbol, bin, fromDate, toDate, provider })
        //     },
        //     currentBacktests(parent, args) {
        //         return [
        //             {
        //                 id: 1,
        //                 start: new Date(),
        //                 end: null,
        //                 progress: 50

        //             }
        //         ]
        //     }
        //     // downloadSecond: DownloadSecondResult,
        //     // downloadMinute: DownloadMinuteResult,
        //     // downloadHour: DownloadHourResult,
        //     // downloadDay: DownloadDayResult,
        //     // downloadDay: DownloadDayResult,
        //   },
        //   Mutation: {
        //       submitBacktest(parent, args, context, info) {
        //         let result = fpp.submitBacktest({
        //             ...args,
        //             fromDate: new Date(args.fromDate).getTime(),
        //             toDate: new Date(args.toDate).getTime()
        //         })
        //         console.log(result.progress);
        //         result.start = new Date()
        //         return result
        //       },
        //       cancelBacktest(parent, args, context, info) {
        //         //   fpp.tick({})
        //           return {
        //               success: true,
        //           }
        //       }
    }
}
