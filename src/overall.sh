bash compile.sh
bash run1.sh $1
minisat "$1.satinput" "$1.satoutput"
bash run2.sh $1
python2 check.py "$1.graphs" "$1.mapping"