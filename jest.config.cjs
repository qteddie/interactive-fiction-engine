module.exports = {
    "transform": {
      "^.+\\.js$": "babel-jest"
    },
    "transformIgnorePatterns": [
      "/node_modules/(?!@babel).+\\.js$"
    ],
    "moduleFileExtensions": [
      "js",
      "json",
      "svelte"
    ],
    "moduleNameMapper": {
      "^.+\\.svelte$": "svelte-jester"
    }
};