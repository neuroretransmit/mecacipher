#!/bin/bash

pushd doc
pdflatex mecacipher.tex
bibtex mecacipher.aux
pdflatex mecacipher.tex
pdflatex mecacipher.tex
popd
#pdflatex --output-directory=./doc ./doc/mecacipher.tex
