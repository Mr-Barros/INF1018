# INF1018
Trabalhos da dupla mais roubada de Software Básico

Link do primeiro trabalho: [text](http://www4.inf.puc-rio.br/~inf1018/2024.2/trabs/t1/trab1-20242.html)

comando para ver arquivo em binário:
xxd -b ./entrada/utf8_peq.txt | cut -d' ' -f2-7 | tr -d ' ' | sed 's/.\{8\}/& /g'