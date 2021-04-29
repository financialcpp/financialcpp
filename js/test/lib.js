const {Financialcpp} = require("../lib");

const assert = require("assert");

function isPromise(value) {
  return value && value.then && typeof value.then === "function";
}

assert(Financialcpp, "The expected module is undefined");

describe("Financialcpp", function () {
  describe("constructor", function () {
    it("should exist", function () {
      assert(Financialcpp.constructor);
    });
  });
});

describe("fpp.subscribe(['symbol1', 'symbol2', ...], callback, options): subscribe to symbols", function () {
  describe("symbols", () => {
    it("pass");
  });
});

const my_call = (...args) => {
  console.log(new Date(), ...args);
};

describe("fpp.download", function () {
  describe("", () => {
    it("should return a promise", async () => {
      let fpp = new Financialcpp();
    });
  });
});

// let fpp = new Financialcpp({test: "yeeee"}, 55);
// fpp.download();
// assert.doesNotThrow(testBasic, undefined, "testBasic threw an expection");
// assert.throws(testInvalidParams, undefined, "testInvalidParams didn't throw");

// console.log("Tests passed- everything looks OK!");
