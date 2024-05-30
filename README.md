# RIPPLE: Bottom-up k-Vertex Connected Component Enumeration by Multiple Extension 
## 1.Setup
This code is based on Stanford Network Analysis Platform (SNAP) library  in C++, and we make some changes for KVCC Enumeration, you can download the changed library by
```
git clone https://github.com/Elssky/Snap-For-KVCC.git
```
Then you can copy this repository to `./Snap-For-KVCC/examples/` by 
```
cd Snap-For-KVCC/exmaples
git clone https://github.com/Elssky/RIPPLE.git
```
## 2.Build
```
cd RIPPLE && make
```
## 3.Run 
Usage:
```
./main [-a BkVCC/VCCE] [-d DBLP] [-k 10] [-s rand/clique/k+1-clique] [-e flow/nbr] [-m flow/old] [-t 1-16](thread num)
```
For exmaples
```
./main -d CA-CondMat -k 13 -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 2
```
