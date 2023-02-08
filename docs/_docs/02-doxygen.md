## Generate Doxygen Documentation

Make sure to install [Doxygen](https://www.doxygen.nl/) and [Graphviz](https://graphviz.org/):

```sh
brew install doxygen graphviz
```

Build the project with the docs

```sh
python3 cli.py configure --docs
python3 cli.py build --docs
python3 -m http.server --directory ./out/build/MacOS-Release/html
```
