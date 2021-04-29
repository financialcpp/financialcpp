let { Financialcpp } = require("bindings")("financialcpp");

// We will augment this when needed
class Fpp extends Financialcpp {
    constructor(...args) {
        super(...args);

        // call .node module base class constructor (pass along the args)
    }
}

module.exports = Fpp