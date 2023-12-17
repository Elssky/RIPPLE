#!/bin/bash

# cd Snap-For-KVCC/examples/testgraph/
# mkdir ./output/DBLP_

# coAuthorsCitesee
# dataset="coAuthorsCitesee"
# # k_list=(9 17 26 34 52 69 86)
# k_list=(7)
# # k_list=(7 9 11 13 15)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_FME_${k}.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s rand -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noS_${k}.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e nbr -t 1 &) >& ./output/$dataset/${dataset}_ME_noE_${k}.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m old -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noM_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_${k}_t=8_new.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo 
# done

# DBLP
# dataset="DBLP"
# k_list=(7 9 11)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s rand -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noS_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e nbr -t 1 &) >& ./output/$dataset/${dataset}_ME_noE_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m old -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noM_${k}.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_FME_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_new_FME_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_TDE_new_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_${k}_t=8_new.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 2 &) >& ./output/$dataset/${dataset}_ME_${k}_t=2.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 4 &) >& ./output/$dataset/${dataset}_ME_${k}_t=4.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_${k}_t=8.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 16 &) >& ./output/$dataset/${dataset}_ME_${k}_t=16.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # echo ./output/$dataset/${dataset}_ME_$k_new.txt
# done


# web-it-2004
# dataset="web-it-2004"
# k_list=(15)
# # k_list=(11 22 43 86	130	173	259	346	431)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_reverse_ME_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s rand -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noS_${k}.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e nbr -t 1 &) >& ./output/$dataset/${dataset}_ME_noE_${k}.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m old -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noM_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 2 &) >& ./output/$dataset/${dataset}_ME_${k}_t=2.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 4 &) >& ./output/$dataset/${dataset}_ME_${k}_t=4.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_${k}_t=8.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 16 &) >& ./output/$dataset/${dataset}_ME_${k}_t=16.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# web-uk-2005
# dataset="web-uk-2005"
# # k_list=(10 25 50 100 150 200 299 399 499)
# k_list=(100)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s rand -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noS_${k}.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e nbr -t 1 &) >& ./output/$dataset/${dataset}_ME_noE_${k}.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m old -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noM_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_${k}_t=8.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 16 &) >& ./output/$dataset/${dataset}_ME_${k}_t=16.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# web-sk-2005
# dataset="web-sk-2005"
# # k_list=(4 6	8 16 24	32 49 65 81)
# k_list=(8)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_${k}_t=8.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_ME_${k_t=8.txt
# done

# cit-patent
# dataset="cit-patent"
# # k_list=(13 15	17 19 26 38	51 64)
# k_list=(13 15 17)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_reverse_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_new_FME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_bad_new_FME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_TDE_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_TDE_new_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_FME_${k}.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# web-google-dir
# dataset="web-google-dir"
# k_list=(17 18)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# soc-pokec
# dataset="soc-pokec"
# k_list=(9 14 38 )
# # k_list=(38 47 )
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# # ca-MathSciNet
# dataset="ca-MathSciNet"
# # k_list=(2 5	7 10 14	19 24)
# k_list=(5 7 9)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_reverse_ME_$k.txt.
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_new_FME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_bad_new_FME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_TDE_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_TDE_new_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_FME_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 2 &) >& ./output/$dataset/${dataset}_ME_${k}_t=2.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 4 &) >& ./output/$dataset/${dataset}_ME_${k}_t=4.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_${k}_t=8.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 16 &) >& ./output/$dataset/${dataset}_ME_${k}_t=16.txt

# # # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # #  echo ./output/$dataset/${dataset}_ME_t=16_$k.txt
# done


# web-arabic-2005
# dataset="web-arabic-2005"
# # k_list=(6 8	10 20 30 40	61 81 101)
# k_list=(5 7 9 11 13)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_${k}_t=8_new.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done


# CA-CondMat
# dataset="CA-CondMat"
# # k_list=(3	5	8	10	15	20	25)
# # k_list=(5 7 9 11 13)
# k_list=(5 7 9)
# for k in ${k_list[@]}
# do

# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 2 &) >& ./output/$dataset/${dataset}_new_ME_${k}_t=2.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 4 &) >& ./output/$dataset/${dataset}_ME_${k}_t=4.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_${k}_t=8.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 16 &) >& ./output/$dataset/${dataset}_ME_${k}_t=16.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_TDE_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_TDE_new_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e flow -t 1 &) >& ./output/$dataset/${dataset}_FME_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 8 &) >& ./output/$dataset/${dataset}_ME_$k_t=8.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# socfb-Bingham82
# dataset="socfb-Bingham82"
# # k_list=(9 19 28 )
# k_list=(11	23	46)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# email-enron-large
# dataset="email-enron-large"
# k_list=(3 7)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done


# sc-shipsec5
# dataset="sc-shipsec5"
# k_list=(16)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s rand -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noS_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e nbr -t 1 &) >& ./output/$dataset/${dataset}_ME_noE_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m old -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noM_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# ca-HepPh
# dataset="ca-HepPh"
# k_list=(7)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt
# # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# bauru5727
# dataset="bauru5727"
# k_list=(3 4 5 6)
# # k_list=(38 47 )
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# sc-shipsec5

# dataset="sc-shipsec5"
# k_list=(19 21)
# # k_list=(38 47 )
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# socfb-konect
# dataset="socfb-konect"
# k_list=(9)
# # k_list=(7 9 11 13 15)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d socfb-konect_7-core -k $k -a BkVCC -s LkVCS+clique+SG &) >& ./output/$dataset/${dataset}_seed_$k.txt
# (/usr/bin/time -v nohup ./main -d socfb-konect_7-core -k $k -a VCCE &) >& ./output/$dataset/${dataset}_new_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d socfb-konect_7-core -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_new_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d socfb-konect_7-core -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_new_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s rand -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noS_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e nbr -t 1 &) >& ./output/$dataset/${dataset}_ME_noE_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m old -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_noM_${k}.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# web-webbase-2001-all
# dataset="web-webbase-2001-all"
# k_list=(1200 1300 1400)
# # k_list=(7 9 11 13 15)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# Stanford
# dataset="Stanford"
# k_list=(5 10 15)
# # k_list=(7 9 11 13 15)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# web-BerkStan
# dataset="web-BerkStan"
# k_list=(35 40)
# # k_list=(7 9 11 13 15)
# for k in ${k_list[@]}
# do
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

# cnr-2000
# dataset="cnr-2000"
# k_list=(5)
# # k_list=(7 9 11 13 15)
# for k in ${k_list[@]}
# do
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

#sc-msdoor
# dataset="sc-msdoor"
# k_list=(34)
# # k_list=(7 9 11 13 15)
# for k in ${k_list[@]}
# do
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

#bio-pdb1HYS
# dataset="bio-pdb1HYS"
# k_list=(10)
# # k_list=(7 9 11 13 15)
# for k in ${k_list[@]}
# do
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done

#sc-shipsec1
# dataset="sc-shipsec1"
# k_list=(16)
# # k_list=(7 9 11 13 15)
# for k in ${k_list[@]}
# do
# (/usr/bin/time -v nohup ./main -d $dataset -k $k -a VCCE &) >& ./output/$dataset/${dataset}_VCCE_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s rand -m old -e nbr -t 1 &) >& ./output/$dataset/${dataset}_BkVCC_$k.txt
# # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s LkVCS+clique+SG -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_ME_$k.txt
# # # (/usr/bin/time -v nohup ./main -d ${dataset} -k $k -a BkVCC -s SG+clique+LkVCS -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_SME_$k.txt
# # (/usr/bin/time -v nohup ./main -d $dataset -k $k -a BkVCC -s mix -m flow -e ring -t 1 &) >& ./output/$dataset/${dataset}_VCCE-RB_$k.txt

# # # # echo ./output/$dataset/${dataset}_BkVCC_$k.txt
# done