%% algebrabook.cls
%% Copyright 2016 Typesetters.se
%
% This work may be distributed and/or modified under the
% conditions of the LaTeX Project Public License, either version 1.3
% of this license or (at your option) any later version.
% The latest version of this license is in
%   http://www.latex-project.org/lppl.txt
% and version 1.3 or later is part of all distributions of LaTeX
% version 2005/12/01 or later.
%
% This is a free LaTeX text book template from Typesetters.se.
% See algebrabook.tex for an example of how to use this class.
%
% Template has been modified by Eldar Sultanow

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{algebrabook}[2017/02/19 Free LaTeX text book template from 
Typesetters.se]
\LoadClass[11pt,onecolumn,titlepage,twoside,openright]{book}

% Language
%--------------------------------------------------------------------------
%\RequirePackage[swedish]{babel}
\RequirePackage[utf8]{inputenc}

% Geometry
%--------------------------------------------------------------------------
\RequirePackage[top=2.5cm,bottom=2.5cm,left=3cm,right=3cm,
                marginparwidth=1.1cm,marginparsep=2mm]{geometry}

% Fonts
%-------------------------------------------------------------------------
\RequirePackage{kpfonts}

% Colors
%-------------------------------------------------------------------------
\RequirePackage{xcolor}
\definecolor{tsdarkbrown}{RGB}{127,32,2}
\definecolor{tssteelblue}{RGB}{23,131,181}
\definecolor{tsorange}{RGB}{255,138,88}
\definecolor{tsblue}{RGB}{23,74,117}
\definecolor{tsforestgreen}{HTML}{4e833c}
\definecolor{tsyellow}{RGB}{255,185,88}
\definecolor{tsgrey}{RGB}{200,200,200}
\definecolor{wisogreen}{HTML}{4e833c}
\definecolor{black}{RGB}{0,0,0}

% Graphics
%-------------------------------------------------------------------------
\RequirePackage{graphicx}
%\RequirePackage{svg}
\RequirePackage{tikz}
\RequirePackage[pages=some]{background}
\graphicspath{{images/}}
\backgroundsetup{scale=1,color=black,opacity=1.0,angle=0,
contents={\includegraphics[width=\paperwidth,height=\paperheight]{cover}}}

% Captions
%-------------------------------------------------------------------------
\RequirePackage{caption}
\captionsetup{justification=centering,singlelinecheck=true,labelfont={bf},aboveskip=1em,belowskip=2em}

% URLs
%-------------------------------------------------------------------------
\usepackage[
colorlinks=true,
urlcolor=wisogreen,
linkcolor=wisogreen,
citecolor=wisogreen
]{hyperref}

% Header and footer styling
%-------------------------------------------------------------------------
\RequirePackage{fancyhdr}
\setlength{\headheight}{15.0pt}
\fancyhf{}
\fancyhead[LE,RO]{\bfseries\thepage}
\fancyhead[RE]{\it\sf\nouppercase{\rightmark}}
\fancyhead[LO]{\it\sf\nouppercase{\leftmark}}
\renewcommand{\headrulewidth}{0pt}
\renewcommand{\footrulewidth}{0pt}

% Section styling
%--------------------------------------------------------------------------
\RequirePackage{sectsty}
\sectionfont{\sf\color{wisogreen}\sectionrule{4ex}{0pt}{-1ex}{1pt}}

% Chapter styling
%--------------------------------------------------------------------------
\makeatletter

\RequirePackage{contour}

% Numbered chapters (with minitoc)
\renewcommand{\@makechapterhead}[1]{
	\begin{tikzpicture}[remember picture,overlay]
		\node at (current page.north west) {
			\begin{tikzpicture}[remember picture,overlay]
			% Background image
			% trim=left top right bottom
			\node[anchor=north west,inner sep=0pt] at (0,0){
				\includegraphics[angle=180, clip, trim=0cm 2cm 0cm 10cm, 
				width=\paperwidth]{background_wisogreen.pdf}
			};
			% Chapter heading
			\draw[anchor=west, inner sep=0pt, text=white] (-0.1cm,-1.5cm)
				node[white, inner sep=10pt] (0,0) {
					\parbox[t][1.6cm][t]{\paperwidth} {
						\huge\bfseries\sffamily\flushleft\thechapter.
						\color{white}\contourlength{1.8pt}
						\contour{wisogreen}{#1}
					}
				};
			\end{tikzpicture}
		};
	\end{tikzpicture}
	\vspace{4.6cm}
}

% Unnumbered chapters (without minitoc)
\renewcommand{\@makeschapterhead}[1]{
	\begin{tikzpicture}[remember picture,overlay]
		\node at (current page.north west) {
			\begin{tikzpicture}[remember picture,overlay]
			% Background image
			% trim=left top right bottom
			\node[anchor=north west,inner sep=0pt] at (0,0){
				\includegraphics[angle=180, clip, trim=0cm 2cm 0cm 10cm, 
				width=\paperwidth]{background_wisogreen.pdf}
			};
			% Chapter heading
			\draw[anchor=west,inner sep=0pt, text=white] (-0.1cm,-1.5cm)
				node[white, inner sep=10pt] (0,0) {
					\parbox[t][1.6cm][t]{\paperwidth} {
						\huge\bfseries\sffamily\flushleft #1
					}
				};
			\end{tikzpicture}
		};
	\end{tikzpicture}
	\vspace{4.6cm}
}

% Table of contents styling
%--------------------------------------------------------------------------
\RequirePackage{titletoc}
\setcounter{tocdepth}{2}

\titlecontents{chapter}[0cm]
{\addvspace{14pt}\large\sffamily\bfseries}
{\color{wisogreen}\contentslabel[\Large\thecontentslabel]{1.25cm}}{}
{\color{wisogreen}\normalsize\sffamily\bfseries
 \;\titlerule*[.5pc]{.}\;\thecontentspage}

\titlecontents{section}[0cm]
{\addvspace{6pt}\sffamily\bfseries}
{\hypersetup{linkcolor=black}\contentslabel[\thecontentslabel]{1.25cm}}{}
{\sffamily\hfill\color{black}\thecontentspage}[]

% Boxes for theorems
%--------------------------------------------------------------------------
\newenvironment{boxtheorem}
{\vspace{0.5cm}\begin{tcolorbox}
[enhanced jigsaw,colback=black!5,colframe=tsorange]}
{\end{tcolorbox}\vspace{0.5cm}}

\newenvironment{boxdefinition}
{\vspace{0.5cm}\begin{tcolorbox}
[enhanced jigsaw,colback=yellow!10!white,boxrule=0pt,frame hidden,
 borderline west={1.5mm}{-2mm}{tsforestgreen}]}
{\end{tcolorbox}\vspace{0.5cm}}

\newenvironment{boxexercise}
{\vspace{0.5cm}\begin{tcolorbox}
[enhanced jigsaw,colback=tsyellow!20,boxrule=0pt,frame hidden]}
{\end{tcolorbox}\vspace{0.5cm}}

\newenvironment{boxproblem}
{\vspace{0.5cm}\begin{tcolorbox}
[enhanced jigsaw,colback=tsorange!25,boxrule=0pt,frame hidden]}
{\end{tcolorbox}\vspace{0.5cm}}

\newenvironment{boxprogramming}
{\vspace{0.5cm}\begin{tcolorbox}
[enhanced jigsaw,colback=tsblue!10,boxrule=0pt,frame hidden]}
{\end{tcolorbox}\vspace{0.5cm}}

\newenvironment{boxexample}
{\vspace{0.5cm}\begin{tcolorbox}
[enhanced jigsaw,colback=tsgrey!20,boxrule=0pt,frame hidden]}
{\end{tcolorbox}\vspace{0.5cm}}

% Styling of theorems
%--------------------------------------------------------------------------
\RequirePackage{amsthm}

\newtheoremstyle{styletheorem}
{0pt}{0pt}{\normalfont}{0pt}{\small\bf\sffamily\color{tsorange}}{\;}{0.25em}
{\small\sffamily\color{tsorange}\thmname{#1}
 \nobreakspace\thmnumber{\@ifnotempty{#1}{}\@upn{#2}}
 \thmnote{\nobreakspace\the\thm@notefont\sffamily\bfseries\color{black} (#3)}}

\newtheoremstyle{styledefinition}
{0pt}{0pt}{\normalfont}{0pt}{\small\bf\sffamily\color{black}}{\;}{0.25em}
{\small\sffamily\color{tsforestgreen}\thmname{#1}
 \nobreakspace\thmnumber{\@ifnotempty{#1}{}\@upn{#2}}
 \thmnote{\nobreakspace\the\thm@notefont\sffamily\bfseries\color{black} (#3)}}

\newtheoremstyle{styleexercise}
{0pt}{0pt}{\normalfont}{0pt}{\small\bf\sffamily\color{black}}{\;}{0.25em}
{\small\sffamily\color{black}\thmname{#1}
 \nobreakspace\thmnumber{\@ifnotempty{#1}{}\@upn{#2}}
 \thmnote{\nobreakspace\the\thm@notefont\sffamily\bfseries\color{black} (#3)}}

\theoremstyle{styledefinition}
\newtheorem{envdefinition}{Definition}[chapter]
\theoremstyle{styletheorem}
\newtheorem{envtheorem}{Theorem}[chapter]
\newtheorem{envlemma}{Lemma}[chapter]
\theoremstyle{styleexercise}
\newtheorem{envexercise}{Exercise}[chapter]
\newtheorem{envproblem}{Problem}[chapter]
\newtheorem{envprogramming}{Computer exercise}[chapter]
\newtheorem{envexample}{Example}[chapter]

% Environments for theorems
%--------------------------------------------------------------------------
\newenvironment{definition}
{\begin{boxdefinition}\begin{envdefinition}}
{\end{envdefinition}\end{boxdefinition}}

\newenvironment{theorem}
{\begin{boxtheorem}\begin{envtheorem}}
{\end{envtheorem}\end{boxtheorem}}

\newenvironment{corollary}
{\begin{boxtheorem}\begin{envtheorem}}
{\end{envtheorem}\end{boxtheorem}}

\newenvironment{lemma}
{\begin{boxtheorem}\begin{envlemma}}
{\end{envlemma}\end{boxtheorem}}

\newenvironment{exercise}
{\begin{boxexercise}\begin{envexercise}}
{\end{envexercise}\end{boxexercise}}

\newenvironment{problem}
{\begin{boxproblem}\begin{envproblem}}
{\end{envproblem}\end{boxproblem}}

\newenvironment{programming}
{\begin{boxprogramming}\begin{envprogramming}}
{\end{envprogramming}\end{boxprogramming}}

\newenvironment{example}
{\begin{boxexample}\begin{envexample}}
{\end{envexample}\end{boxexample}}

% Other environments
%--------------------------------------------------------------------------
\newenvironment{summary}
{\hspace{10pt}\par\small\it
 \begin{list}{}{\leftmargin=35pt\rightmargin=25pt}
 \item\ignorespaces\advance\baselineskip -1pt}
{\end{list}\vspace{-0.5cm}}

% avoid leading space when using custom environment
% https://latex.org/forum/viewtopic.php?t=6368
\newenvironment{remark}
{\vspace{0.5cm}\pagebreak[0]\noindent\small\it\marginpar{\vspace{-3mm}\includegraphics[width=1.0cm]{idea}}\ignorespaces}
{\vspace{0.5cm}\ignorespacesafterend}

% Index
%--------------------------------------------------------------------------
%\RequirePackage{makeidx}
%\makeindex

% Packages for mathematics and algorithms
%--------------------------------------------------------------------------
\RequirePackage{algorithm}
\RequirePackage{algpseudocode}
\RequirePackage{xfrac}
\RequirePackage{mathtools}
\RequirePackage{wasysym}
\RequirePackage{siunitx}
\newcommand\bmmax{2}
\RequirePackage{bm}

% Other useful packages
%--------------------------------------------------------------------------
\RequirePackage{blindtext}
\RequirePackage[most]{tcolorbox}
\RequirePackage{booktabs}
\RequirePackage{url}
