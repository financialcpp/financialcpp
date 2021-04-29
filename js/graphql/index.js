const { ApolloServer } = require('apollo-server-express')

const { typeDefs } = require('./typeDefs.js')
const { resolvers } = require('./resolvers.js')

const apolloDefaultOptions = {
    typeDefs,
    resolvers
}

module.exports = ({ apollo = {} } = {}) => {
    return new ApolloServer({
        ...apolloDefaultOptions,
        ...apollo,
    })
}