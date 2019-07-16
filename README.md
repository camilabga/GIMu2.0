# Milker Robot for the 2017 IEEE Latin American Robotics Competition 
Os desafios propostos pela IEEE Latin American Robotics Competition têm como objetivo levar às competições problemas reais encontrados no cotidiano das pessoas e indústrias. Nas edições de 2016 e 2017 da competição é abordada a questão da qualidade na produção do setor alimentício.

O processo atual da produção de leite envolve muito estresse sobre os animais, o que acarreta em um leite de qualidade inferior. Podemos melhorar isso ao mudar alguns fatores. A  proposta desse desafio é fazer com que o habitat natural (pasto) deixe o animal mais confortável, para que assim os níveis de estresse do animal diminuam. 

O problema proposto apresenta um cenário interessante para explorar técnicas de processamento de imagens, principalmente para a localização da miniatura de vaca. O método desenvolvido tem como saída o centro de massa do objeto e é baseado em alguns algoritmos conhecidos, como Shi-Tomasi Corner Detector, o clustering K-means e local binary patterns.

Dado o problema, propomos uma solução: um robô com uma garra e um sistema de visão que também será responsável pela ordenha e transporte do leite. Com recursos como Arduino, Raspberry Pi, OpenCV e ArUco será possível apresentar essa solução.

Segue o artigo que descreve o algoritmo de detecção da minitura de vaca: https://ieeexplore.ieee.org/document/8215302
