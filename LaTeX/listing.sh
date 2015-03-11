#! /bin/bash

files=($(while (($# != 0)); do
	find "$1" -type f -name '*.cpp'
	find "$1" -type f -name '*.c'
	find "$1" -type f -name '*.h'
	shift
done | sort))

echo '\documentclass[a4paper,notitlepage,10pt]{report}
\usepackage[left=2cm,right=2cm,top=2.5cm,bottom=2.5cm,footskip=1.25cm]{geometry}
\usepackage{listings}
\usepackage{color}

\definecolor{dkgreen}{rgb}{0,0.6,0}
\definecolor{gray}{rgb}{0.5,0.5,0.5}
\definecolor{mauve}{rgb}{0.58,0,0.82}

\lstset{frame=tb,
  language=C++,
  aboveskip=2mm,
  belowskip=2mm,
  showstringspaces=false,
  columns=flexible,
  basicstyle={\small\ttfamily},
  numbers=left,
  numberstyle=\tiny\color{gray},
  keywordstyle=\color{blue},
  commentstyle=\color{dkgreen},
  stringstyle=\color{mauve},
  breaklines=true,
  breakatwhitespace=true,
  tabsize=8
}

\begin{document}'

for ((i = 0; i < ${#files[@]}; i++)); do
	echo '\begin{lstlisting}'
	echo "// ${files[i]}"
	cat "${files[i]}"
	echo '\end{lstlisting}'
	echo
done

echo '\end{document}'
