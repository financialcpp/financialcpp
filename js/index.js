const Financialcpp = require('./lib')

const FppServer = require('./graphql/index.js')
const { resolvers } = require('./graphql/resolvers.js')
const { typeDefs } = require('./graphql/typeDefs.js')

module.exports = {
    Financialcpp,
    FppServer,
    graphql: {
        resolvers,
        typeDefs
    }
};
