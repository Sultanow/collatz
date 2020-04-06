%% Version, November 1, 2015
%% November 1, 2015
%****************************************************************%
%*                                                              *%
%*   SciPress                                                   *%
%*   SciPress LaTeX Document Class                              *%
%*                                                              *%
%*   SciPress                                                   *%
%*   Seestrasse 24c                                             *%
%*   CH-8806 Bach                                               *%
%*   Switzerland                                                *%
%*   Fax +41 (4) 5000-600                                       *%
%*   E-Mail info@scipress.com                                   *%
%*   http://www.scipress.com                                    *%
%*                                                              *%
%*   This style file is an adaptation of                        *%
%*   the original LaTeX article.cls                             *%
%*                                                              *%
%*   Updated by Alex Shchitov.                                  *%
%*                                                              *%
%*   Use:  \documentclass{SciPress_2015}                        *%
%*                                                              *%
%****************************************************************%


\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{SciPress}
              [2015/11/01
SciPress]

%%% from SciPress.sty

\def\@mbi{cmmib10}
\def\@ptsize{0} \@namedef{ds@11pt}{\def\@ptsize{1}}
\@namedef{ds@12pt}{\def\@ptsize{2}}
\def\ds@twoside{\@twosidetrue \@mparswitchtrue}
\def\ds@draft{\overfullrule 5pt}
\newif\if@envcountreset\@envcountresetfalse
\def\ds@envcountreset{\@envcountresettrue}

%%% end from SciPress.sty

\newif\if@restonecol
\newif\if@titlepage
\@titlepagefalse

\DeclareOption{landscape}
   {\setlength\@tempdima   {\paperheight}%
    \setlength\paperheight {\paperwidth}%
    \setlength\paperwidth  {\@tempdima}}

\if@compatibility
  \renewcommand\@ptsize{0}
\else
\DeclareOption{10pt}{\renewcommand\@ptsize{0}}
\fi
\DeclareOption{11pt}{\renewcommand\@ptsize{1}}
\DeclareOption{12pt}{\renewcommand\@ptsize{2}}
\if@compatibility\else
\DeclareOption{oneside}{\@twosidefalse \@mparswitchfalse}
\fi
\DeclareOption{twoside}{\@twosidetrue  \@mparswitchtrue}
\DeclareOption{draft}{\setlength\overfullrule{5pt}}
\if@compatibility\else
\DeclareOption{final}{\setlength\overfullrule{0pt}}
\fi
\DeclareOption{titlepage}{\@titlepagetrue}
\if@compatibility\else
\DeclareOption{notitlepage}{\@titlepagefalse}
\fi
\if@compatibility\else
\DeclareOption{onecolumn}{\@twocolumnfalse}
\fi
\DeclareOption{twocolumn}{\@twocolumntrue}
\DeclareOption{leqno}{\input{leqno.clo}}
\DeclareOption{fleqn}{\input{fleqn.clo}}
\DeclareOption{openbib}{%
  \AtEndOfPackage{%
   \renewcommand\@openbib@code{%
      \advance\leftmargin\bibindent
      \itemindent -\bibindent
      \listparindent \itemindent
      \parsep \z@
      }%
   \renewcommand\newblock{\par}}%
}
\ExecuteOptions{letterpaper,twoside,onecolumn,final}
\ProcessOptions
%%% size12.clo
\renewcommand\normalsize{%
   \@setfontsize\normalsize\@xiipt{14.5}%
   \abovedisplayskip 12\p@ \@plus3\p@ \@minus7\p@
   \abovedisplayshortskip \z@ \@plus3\p@
   \belowdisplayshortskip 6.5\p@ \@plus3.5\p@ \@minus3\p@
   \belowdisplayskip \abovedisplayskip
   \let\@listi\@listI}
\normalsize
\newcommand\small{%
   \@setfontsize\small\@xipt{13.6}%
   \abovedisplayskip 11\p@ \@plus3\p@ \@minus6\p@
   \abovedisplayshortskip \z@ \@plus3\p@
   \belowdisplayshortskip 6.5\p@ \@plus3.5\p@ \@minus3\p@
   \def\@listi{\leftmargin\leftmargini
               \topsep 9\p@ \@plus3\p@ \@minus5\p@
               \parsep 4.5\p@ \@plus2\p@ \@minus\p@
               \itemsep \parsep}%
   \belowdisplayskip \abovedisplayskip
}
\newcommand\footnotesize{%
   \@setfontsize\footnotesize\@xpt\@xiipt
   \abovedisplayskip 10\p@ \@plus2\p@ \@minus5\p@
   \abovedisplayshortskip \z@ \@plus3\p@
   \belowdisplayshortskip 6\p@ \@plus3\p@ \@minus3\p@
   \def\@listi{\leftmargin\leftmargini
               \topsep 6\p@ \@plus2\p@ \@minus2\p@
               \parsep 3\p@ \@plus2\p@ \@minus\p@
               \itemsep \parsep}%
   \belowdisplayskip \abovedisplayskip
}
\newcommand\scriptsize{\@setfontsize\scriptsize\@viiipt{9.5}}
\newcommand\tiny{\@setfontsize\tiny\@vipt\@viipt}
\newcommand\large{\@setfontsize\large\@xivpt{18}}
\newcommand\Large{\@setfontsize\Large\@xviipt{22}}
\newcommand\LARGE{\@setfontsize\LARGE\@xxpt{25}}
\newcommand\huge{\@setfontsize\huge\@xxvpt{30}}
\let\Huge=\huge
\if@twocolumn
  \setlength\parindent{1em}
\else
  \setlength\parindent{1.5em}
\fi
\setlength\smallskipamount{3\p@ \@plus 1\p@ \@minus 1\p@}
\setlength\medskipamount{6\p@ \@plus 2\p@ \@minus 2\p@}
\setlength\bigskipamount{12\p@ \@plus 4\p@ \@minus 4\p@}
\setlength\headheight{12\p@}
\setlength\headsep   {25\p@}
\setlength\topskip   {12\p@}
\setlength\footskip{30\p@}

\setlength\maxdepth{.5\topskip}

  \setlength\@tempdima{\paperwidth}
  \addtolength\@tempdima{-2in}
  \setlength\@tempdimb{390\p@}
    \ifdim\@tempdima>\@tempdimb\relax
      \setlength\textwidth{\@tempdimb}
    \else
      \setlength\textwidth{\@tempdima}
    \fi


\if@twocolumn
 \setlength\marginparsep {10\p@}
\else
  \setlength\marginparsep{10\p@}
\fi

\setlength\marginparpush{7\p@}

  \if@twoside
    \setlength\@tempdima        {\paperwidth}
    \addtolength\@tempdima      {-\textwidth}
    \setlength\oddsidemargin    {.4\@tempdima}
    \addtolength\oddsidemargin  {-1in}
    \setlength\marginparwidth   {.6\@tempdima}
    \addtolength\marginparwidth {-\marginparsep}
    \addtolength\marginparwidth {-0.4in}

  \ifdim \marginparwidth >2in
     \setlength\marginparwidth{2in}
  \fi
  \@settopoint\oddsidemargin
  \@settopoint\marginparwidth
  \setlength\evensidemargin  {\paperwidth}
  \addtolength\evensidemargin{-2in}
  \addtolength\evensidemargin{-\textwidth}
  \addtolength\evensidemargin{-\oddsidemargin}
  \@settopoint\evensidemargin
\fi

  \setlength\topmargin{\paperheight}
  \addtolength\topmargin{-2in}
  \addtolength\topmargin{-\headheight}
  \addtolength\topmargin{-\headsep}
  \addtolength\topmargin{-\textheight}
  \addtolength\topmargin{-\footskip}     % this might be wrong!
  \addtolength\topmargin{-.5\topmargin}
  \@settopoint\topmargin

\setlength\footnotesep{8.4\p@}
\setlength{\skip\footins}{10.8\p@ \@plus 4\p@ \@minus 2\p@}
\setlength\floatsep    {12\p@ \@plus 2\p@ \@minus 4\p@}
\setlength\textfloatsep{20\p@ \@plus 2\p@ \@minus 4\p@}
\setlength\intextsep   {14\p@ \@plus 4\p@ \@minus 4\p@}
\setlength\dblfloatsep    {14\p@ \@plus 2\p@ \@minus 4\p@}
\setlength\dbltextfloatsep{20\p@ \@plus 2\p@ \@minus 4\p@}
\setlength\@fptop{0\p@ \@plus 1fil}
\setlength\@fpsep{10\p@ \@plus 2fil}
\setlength\@fpbot{0\p@ \@plus 1fil}
\setlength\@dblfptop{0\p@ \@plus 1fil}
\setlength\@dblfpsep{10\p@ \@plus 2fil}
\setlength\@dblfpbot{0\p@ \@plus 1fil}
\setlength\partopsep{3\p@ \@plus 2\p@ \@minus 2\p@}
\def\@listi{\leftmargin\leftmargini
            \parsep 5\p@  \@plus2.5\p@ \@minus\p@
            \topsep 10\p@ \@plus4\p@   \@minus6\p@
            \itemsep5\p@  \@plus2.5\p@ \@minus\p@}
\let\@listI\@listi
\@listi
\def\@listii {\leftmargin\leftmarginii
              \labelwidth\leftmarginii
              \advance\labelwidth-\labelsep
              \topsep    5\p@   \@plus2.5\p@ \@minus\p@
              \parsep    2.5\p@ \@plus\p@    \@minus\p@
              \itemsep   \parsep}
\def\@listiii{\leftmargin\leftmarginiii
              \labelwidth\leftmarginiii
              \advance\labelwidth-\labelsep
              \topsep    2.5\p@\@plus\p@\@minus\p@
              \parsep    \z@
              \partopsep \p@ \@plus\z@ \@minus\p@
              \itemsep   \topsep}
\def\@listiv {\leftmargin\leftmarginiv
              \labelwidth\leftmarginiv
              \advance\labelwidth-\labelsep}
\def\@listv  {\leftmargin\leftmarginv
              \labelwidth\leftmarginv
              \advance\labelwidth-\labelsep}
\def\@listvi {\leftmargin\leftmarginvi
              \labelwidth\leftmarginvi
              \advance\labelwidth-\labelsep}


%%%%%%%%%%%%%%

\setlength\lineskip{1\p@}
\setlength\normallineskip{1\p@}
\renewcommand\baselinestretch{}
\setlength\parskip{0\p@ \@plus \p@}
\@lowpenalty   51
\@medpenalty  151
\@highpenalty 301
\setcounter{topnumber}{2}
\renewcommand\topfraction{.7}
\setcounter{bottomnumber}{1}
\renewcommand\bottomfraction{.3}
\setcounter{totalnumber}{3}
\renewcommand\textfraction{.2}
\renewcommand\floatpagefraction{.5}
\setcounter{dbltopnumber}{2}
\renewcommand\dbltopfraction{.7}
\renewcommand\dblfloatpagefraction{.5}


\setcounter{secnumdepth}{0}
\newcounter {part}
\newcounter {section}
\newcounter {subsection}[section]
\newcounter {subsubsection}[subsection]
\newcounter {paragraph}[subsubsection]
\newcounter {subparagraph}[paragraph]
\renewcommand \thepart {\@Roman\c@part}
\renewcommand \thesection {\@arabic\c@section}
\renewcommand\thesubsection   {\thesection.\@arabic\c@subsection}
\renewcommand\thesubsubsection{\thesubsection .\@arabic\c@subsubsection}
\renewcommand\theparagraph    {\thesubsubsection.\@arabic\c@paragraph}
\renewcommand\thesubparagraph {\theparagraph.\@arabic\c@subparagraph}

\newcommand\section{\@startsection {section}{1}{\z@}%
                                   {-3.5ex \@plus -1ex \@minus -.2ex}%
                                   {2.3ex \@plus.2ex}%
                                   {\normalfont\normalsize\bfseries} }
\newcommand\subsection{\@startsection{subsection}{2}{\z@}%
                                    {3.25ex \@plus1ex \@minus.2ex}%
                                    {-1em}%
                                     {\normalfont\normalsize\bfseries}}
\newcommand\subsubsection{\@startsection{subsubsection}{3}{\z@}%
                                     {-3.25ex\@plus -1ex \@minus -.2ex}%
                                     {1.5ex \@plus .2ex}%
                                     {\normalfont\normalsize\bfseries}}
\newcommand\paragraph{\@startsection{paragraph}{4}{\z@}%
                                    {3.25ex \@plus1ex \@minus.2ex}%
                                    {-1em}%
                                    {\normalfont\normalsize\bfseries}}
\newcommand\subparagraph{\@startsection{subparagraph}{5}{\parindent}%
                                       {3.25ex \@plus1ex \@minus .2ex}%
                                       {-1em}%
                                      {\normalfont\normalsize\bfseries}}

\setlength\leftmargini  {2.5em}

\leftmargin  \leftmargini
\setlength\leftmarginii  {2.2em}
\setlength\leftmarginiii {1.87em}
\setlength\leftmarginiv  {1.7em}

  \setlength\leftmarginv  {1em}
  \setlength\leftmarginvi {1em}

\setlength  \labelsep  {.5em}
\setlength  \labelwidth{\leftmargini}
\addtolength\labelwidth{-\labelsep}
\@beginparpenalty -\@lowpenalty
\@endparpenalty   -\@lowpenalty
\@itempenalty     -\@lowpenalty
\renewcommand\theenumi{\@arabic\c@enumi}
\renewcommand\theenumii{\@alph\c@enumii}
\renewcommand\theenumiii{\@roman\c@enumiii}
\renewcommand\theenumiv{\@Alph\c@enumiv}
\newcommand\labelenumi{\theenumi.}
\newcommand\labelenumii{(\theenumii)}
\newcommand\labelenumiii{\theenumiii.}
\newcommand\labelenumiv{\theenumiv.}
\renewcommand\p@enumii{\theenumi}
\renewcommand\p@enumiii{\theenumi(\theenumii)}
\renewcommand\p@enumiv{\p@enumiii\theenumiii}
\newcommand\labelitemi{\textbullet}
\newcommand\labelitemii{\normalfont\bfseries \textendash}
\newcommand\labelitemiii{\textasteriskcentered}
\newcommand\labelitemiv{\textperiodcentered}
\newenvironment{description}
               {\list{}{\labelwidth\z@ \itemindent-\leftmargin
                        \let\makelabel\descriptionlabel}}
               {\endlist}
\newcommand*\descriptionlabel[1]{\hspace\labelsep
                                \normalfont\bfseries #1}

\newenvironment{verse}
               {\let\\\@centercr
                \list{}{\itemsep      \z@
                        \itemindent   -1.5em%
                        \listparindent\itemindent
                        \rightmargin  \leftmargin
                        \advance\leftmargin 1.5em}%
                \item\relax}
               {\endlist}
\newenvironment{quotation}
               {\list{}{\listparindent 1.5em%
                        \itemindent    \listparindent
                        \rightmargin   \leftmargin
                        \parsep        \z@ \@plus\p@}%
                \item\relax}
               {\endlist}
\newenvironment{quote}
               {\list{}{\rightmargin\leftmargin}%
                \item\relax}
               {\endlist}
\if@compatibility
\newenvironment{titlepage}
    {%
      \if@twocolumn
        \@restonecoltrue\onecolumn
      \else
        \@restonecolfalse\newpage
      \fi
      \setcounter{page}\z@
    }%
    {\if@restonecol\twocolumn \else \newpage \fi
    }
\else
\newenvironment{titlepage}
    {%
      \if@twocolumn
        \@restonecoltrue\onecolumn
      \else
        \@restonecolfalse\newpage
      \fi
      \setcounter{page}\@ne
    }%
    {\if@restonecol\twocolumn \else \newpage \fi
     \if@twoside\else
        \setcounter{page}\@ne
     \fi
    }
\fi
\newcommand\appendix{\par
  \setcounter{section}{0}%
  \setcounter{subsection}{0}%
  \gdef\thesection{\@Alph\c@section}}
\setlength\arraycolsep{5\p@}
\setlength\tabcolsep{6\p@}
\setlength\arrayrulewidth{.4\p@}
\setlength\doublerulesep{2\p@}
\setlength\tabbingsep{\labelsep}
\skip\@mpfootins = \skip\footins
\setlength\fboxsep{3\p@}
\setlength\fboxrule{.4\p@}
\renewcommand \theequation {\@arabic\c@equation}
\newcounter{figure}
\renewcommand \thefigure {\@arabic\c@figure}
\def\fps@figure{tbp}
\def\ftype@figure{1}
\def\ext@figure{lof}
\def\fnum@figure{\figurename~\thefigure}
\newenvironment{figure}
               {\@float{figure}}
               {\end@float}
\newenvironment{figure*}
               {\@dblfloat{figure}}
               {\end@dblfloat}
\newcounter{table}
\renewcommand\thetable{\@arabic\c@table}
\def\fps@table{tbp}
\def\ftype@table{2}
\def\ext@table{lot}
\def\fnum@table{\tablename~\thetable}
\newenvironment{table}
               {\@float{table}}
               {\end@float}
\newenvironment{table*}
               {\@dblfloat{table}}
               {\end@dblfloat}
\newlength\abovecaptionskip
\newlength\belowcaptionskip
\setlength\abovecaptionskip{10\p@}
\setlength\belowcaptionskip{0\p@}
\long\def\@makecaption#1#2{%
  \vskip\abovecaptionskip
  \sbox\@tempboxa{#1: #2}%
  \ifdim \wd\@tempboxa >\hsize
    #1: #2\par
  \else
    \global \@minipagefalse
    \hb@xt@\hsize{\hfil\box\@tempboxa\hfil}%
  \fi
  \vskip\belowcaptionskip}
\DeclareOldFontCommand{\rm}{\normalfont\rmfamily}{\mathrm}
\DeclareOldFontCommand{\sf}{\normalfont\sffamily}{\mathsf}
\DeclareOldFontCommand{\tt}{\normalfont\ttfamily}{\mathtt}
\DeclareOldFontCommand{\bf}{\normalfont\bfseries}{\mathbf}
\DeclareOldFontCommand{\it}{\normalfont\itshape}{\mathit}
\DeclareOldFontCommand{\sl}{\normalfont\slshape}{\@nomath\sl}
\DeclareOldFontCommand{\sc}{\normalfont\scshape}{\@nomath\sc}
\DeclareRobustCommand*\cal{\@fontswitch\relax\mathcal}
\DeclareRobustCommand*\mit{\@fontswitch\relax\mathnormal}
\newcommand\@pnumwidth{1.55em}
\newcommand\@tocrmarg{2.55em}
\newcommand\@dotsep{4.5}
\setcounter{tocdepth}{3}
\newcommand\tableofcontents{%
    \section*{\contentsname
        \@mkboth{%
           \MakeUppercase\contentsname}{\MakeUppercase\contentsname}}%
    \@starttoc{toc}%
    }
\newcommand*\l@part[2]{%
  \ifnum \c@tocdepth >-2\relax
    \addpenalty\@secpenalty
    \addvspace{2.25em \@plus\p@}%
    \begingroup
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      {\leavevmode
       \large \bfseries #1\hfil \hb@xt@\@pnumwidth{\hss #2}}\par
       \nobreak
       \if@compatibility
         \global\@nobreaktrue
         \everypar{\global\@nobreakfalse\everypar{}}%
      \fi
    \endgroup
  \fi}
\newcommand*\l@section[2]{%
  \ifnum \c@tocdepth >\z@
    \addpenalty\@secpenalty
    \addvspace{1.0em \@plus\p@}%
    \setlength\@tempdima{1.5em}%
    \begingroup
      \parindent \z@ \rightskip \@pnumwidth
      \parfillskip -\@pnumwidth
      \leavevmode \bfseries
      \advance\leftskip\@tempdima
      \hskip -\leftskip
      #1\nobreak\hfil \nobreak\hb@xt@\@pnumwidth{\hss #2}\par
    \endgroup
  \fi}
\newcommand*\l@subsection{\@dottedtocline{2}{1.5em}{2.3em}}
\newcommand*\l@subsubsection{\@dottedtocline{3}{3.8em}{3.2em}}
\newcommand*\l@paragraph{\@dottedtocline{4}{7.0em}{4.1em}}
\newcommand*\l@subparagraph{\@dottedtocline{5}{10em}{5em}}
\newcommand\listoffigures{%
    \section*{\listfigurename
      \@mkboth{\MakeUppercase\listfigurename}%
              {\MakeUppercase\listfigurename}}%
    \@starttoc{lof}%
    }
\newcommand*\l@figure{\@dottedtocline{1}{1.5em}{2.3em}}
\newcommand\listoftables{%
    \section*{\listtablename
      \@mkboth{%
          \MakeUppercase\listtablename}{\MakeUppercase\listtablename}}%
    \@starttoc{lot}%
    }
\let\l@table\l@figure
\newdimen\bibindent
\setlength\bibindent{1.5em}

%% thebibliography is defined above, keep this for reference
\newenvironment{xthebibliography}[1]
     {\section*{\refname
        \@mkboth{\MakeUppercase\refname}{\MakeUppercase\refname}}%
      \list{\@biblabel{\@arabic\c@enumiv}}%
           {\settowidth\labelwidth{\@biblabel{#1}}%
            \leftmargin\labelwidth
            \advance\leftmargin\labelsep
            \@openbib@code
            \usecounter{enumiv}%
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000%
      \sfcode`\.\@m}
     {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
      \endlist}
\newcommand\newblock{\hskip .11em\@plus.33em\@minus.07em}
\let\@openbib@code\@empty
\newenvironment{theindex}
               {\if@twocolumn
                  \@restonecolfalse
                \else
                  \@restonecoltrue
                \fi
                \columnseprule \z@
                \columnsep 35\p@
                \twocolumn[\section*{\indexname}]%
                \@mkboth{\MakeUppercase\indexname}%
                        {\MakeUppercase\indexname}%
                \thispagestyle{plain}\parindent\z@
                \parskip\z@ \@plus .3\p@\relax
                \let\item\@idxitem}
               {\if@restonecol\onecolumn\else\clearpage\fi}
\newcommand\@idxitem{\par\hangindent 40\p@}
\newcommand\subitem{\@idxitem \hspace*{20\p@}}
\newcommand\subsubitem{\@idxitem \hspace*{30\p@}}
\newcommand\indexspace{\par \vskip 10\p@ \@plus5\p@ \@minus3\p@\relax}
\renewcommand\footnoterule{%
  \kern-3\p@
  \hrule\@width.4\columnwidth
  \kern2.6\p@}
\newcommand\@makefntext[1]{%
    \parindent 1em%
    \noindent
    \hb@xt@1.8em{\hss\@makefnmark}#1}


%\newcommand\contentsname{Contents}
%\newcommand\listfigurename{List of Figures}
%\newcommand\listtablename{List of Tables}
\newcommand\refname{References}
\newcommand\indexname{Index}
%\newcommand\figurename{Figure}
%\newcommand\tablename{Table}
\newcommand\partname{Part}
\newcommand\appendixname{Appendix}
%\newcommand\abstractname{Abstract}

\setlength\columnsep{10\p@}
\setlength\columnseprule{0\p@}

\pagenumbering{arabic}
\if@twoside
\else
  \raggedbottom
\fi
\if@twocolumn
  \twocolumn
  \sloppy
  \flushbottom
\else
  \onecolumn
\fi

%%%%%%%%%%%%%%%%%%%%% additions from

\def\copyrightyear#1{\def\thecopyrightyear{#1}}

\def\journalcitation#1{\def\thejournalcitation{#1\\
Online available at: http://www.scipress.com \\
Copyright \thecopyrightyear\ SciPress, Switzerland
}}
\def\publicationtitle#1{\def\thepublicationtitle{#1}}

%%%%%%%%%%%%%%%%%%%%% additions from scipress.sty
% LaTeX does not provide a command to enter the authors institute
% addresses. The \institute command is defined here.

\oddsidemargin -15pt
\evensidemargin -15pt
\marginparwidth 9pt
\marginparsep 10pt

\topmargin -25pt \headheight 12pt \headsep 13.66pt
%\footheight 12pt
 \footskip 30pt
\textwidth 17cm
\textheight 24cm
\columnsep 1cc \columnseprule 0pt

% \footnotesep 7.7pt
% \skip\footins 9pt plus 4pt minus 2pt
% \floatsep 12pt plus 2pt minus 2pt
% \textfloatsep 8mm plus 2pt minus 4pt
% \intextsep 8mm plus 2pt minus 2pt
% \@maxsep 8mm
% \dblfloatsep 12pt plus 2pt minus 2pt
% \dbltextfloatsep 20pt plus 2pt minus 4pt
% \@dblmaxsep 20pt
% \@fptop 0pt plus 1fil \@fpsep 8pt plus 2fil \@fpbot 0pt plus 1fil
% \@dblfptop0pt plus1fil \@dblfpsep8pt plus2fil\@dblfpbot 0pt plus1fil
% \marginparpush 5pt

\footnotesep 8.4pt
\skip\footins 10.8pt plus 4pt minus 2pt
\floatsep 14pt plus 2pt minus 4pt
\textfloatsep 20pt plus 2pt minus 4pt
\intextsep 14pt plus 4pt minus 4pt
%\@maxsep 20pt
\dblfloatsep 14pt plus 2pt minus 4pt
\dbltextfloatsep 20pt plus 2pt minus 4pt
%\@dblmaxsep 20pt
\@fptop 0pt plus 1fil \@fpsep 10pt plus 2fil \@fpbot 0pt plus 1fil
\@dblfptop 0pt plus 1fil \@dblfpsep 10pt plus 2fil \@dblfpbot 0pt plus 1fil
\marginparpush 7pt

% \parskip 0pt plus 1pt \parindent 1.5em \topsep 8pt plus 2pt minus 4pt

\parskip 0pt plus 1pt \parindent 1.5em \partopsep 3pt plus 2pt minus 2pt

%
%    atencao `as linhas de baixo! nao foram substituidas !!!!!
%
\partopsep 2pt plus 1pt minus 1pt \itemsep \z@
\@lowpenalty 51 \@medpenalty 151 \@highpenalty 301
\@beginparpenalty-\@lowpenalty\@endparpenalty -\@lowpenalty\@itempenalty
-\@lowpenalty

\def\vec#1{\mathchoice{\mbox{\boldmath$\displaystyle\bf#1$}}
{\mbox{\boldmath$\textstyle\bf#1$}}
{\mbox{\boldmath$\scriptstyle\bf#1$}}
{\mbox{\boldmath$\scriptscriptstyle\bf#1$}}}

\newcounter{@inst}
\newcounter{@auth}

\def\institute#1{\gdef\@institute{#1}}

\def\institutename{\par
 \begingroup
 \parskip=\z@
 \parindent=\z@
 \setcounter{@inst}{1}%
 \def\and{\par\vspace{6pt}\stepcounter{@inst}%
 \noindent$^{\the@inst}$\ignorespaces}%
 \setbox0=\vbox{\def\thanks##1{}\@institute}%
 \ifnum\c@@inst=1\relax
 \else
   \setcounter{footnote}{\c@@inst}%
   \setcounter{@inst}{1}%
   \noindent$^{\the@inst}$%\enspace
 \fi
 \ignorespaces
 \@institute\par
 \endgroup}

\def\@thanks{}

\def\@fnsymbol#1{\ifcase#1\or\star\or{\star\star}\or{\star\star\star}%
   \or \dagger\or \ddagger\or
   \mathchar "278\or \mathchar "27B\or \|\or **\or \dagger\dagger
   \or \ddagger\ddagger\else\@ctrerr\fi\relax}

\def\inst#1{\unskip$^{#1}$}

\def\subtitle#1{\gdef\@subtitle{#1}}
\def\@subtitle{}

\def\maketitle{\par
 \begingroup
 \parindent=\z@
 \def\thefootnote{\fnsymbol{footnote}}
 \newpage
 \global\@topnum\z@ \@maketitle \@thanks
 \endgroup
 \let\maketitle\relax
 \let\@maketitle\relax
 \gdef\@thanks{}\gdef\@author{}\gdef\@title{}\gdef\@subtitle{}%
 \let\thanks\relax}

\def\@maketitle{\newpage
 \begin{center}%
 {\large\boldmath \titlefont
  \pretolerance=10000
  \@title \vskip 6pt}
\if!\@subtitle!\else {\Large \bf\titlefont
  \vskip -.65cm
  \pretolerance=10000
  \@subtitle \par}\vskip .8cm\fi
{\normalsize\rm\lineskip .5em
\authorfont\@author\vskip 6pt}
 {\small\rm\addressfont\institutename}\vskip 6pt
 \end{center}%
 }

\mark{{}{}}

% Define `keyword' environment
\def\keywords{%
%\list{}{
\advance\topsep by0.65cm\relax
\vskip\topsep
{\keywordfont
% \leftmargin=1cm
 \labelwidth=\z@
 \listparindent=\z@
 \itemindent\listparindent
% \rightmargin\leftmargin
}%\item[\hskip\labelsep
\noindent\underline{\keywordnamefont\keywordname}%]
}
%\let\endkeywords=\endlist
\let\endkeywords=\relax
\def\quote{\list{}{\rightmargin\leftmargin}\item[]}
\let\endquote=\endlist

% Define `abstract' environment
\def\abstract{%
%\list{}{
\advance\topsep by0.65cm\relax
\vskip\topsep
%{\abstractfont
 %\leftmargin=1cm
 %\labelwidth=\z@
% \listparindent=\z@
% \itemindent\listparindent
% \rightmargin\leftmargin
%}
%\item[\hskip\labelsep
\abstractfont
\noindent{\abstractnamefont\abstractname}%]
}
%\let\endabstract=\endlist
\let\endabstract=\relax
\def\quote{\list{}{\rightmargin\leftmargin}\item[]}
\let\endquote=\endlist

\newif\iftitlepage
\global\titlepagetrue

\def\ps@headings{%
\def\@oddhead{%
\hbox to\textwidth{\iftitlepage
  \vbox to0pt{\vss\journamefont
  \noindent\thejournalcitation\\
%  Copyright by SciPress
%  \vskip12pt
\vskip.1pt}\hfill
\else%
  \vbox to0pt{\vss\hbox to\textwidth{%
  \hfill\runningheadfont\thepublicationtitle\hfill
  \llap{\foliofont\the\c@page}}
\vskip4pt
\hrule height .8pt}\fi}}
%%
\def\@evenhead{%
\hbox to\textwidth{\hss
\vbox to 0pt{\vss
\hbox to\textwidth{%
\rlap{\foliofont\the\c@page}\hfill\runningheadfont\thepublicationtitle\hfill}
\vskip4pt
\hrule height .8pt}\hss}}
%%
\def\@evenfoot{\global\titlepagefalse\hfill}
\let\@oddfoot\@evenfoot
\def\sectionmark##1{}%
\def\subsectionmark##1{}}

\def\today{\ifcase\month\or
 January\or February\or March\or April\or May\or June\or
 July\or August\or September\or October\or November\or December\fi
 \space\number\day, \number\year}

%\ps@headings \pagenumbering{arabic} \onecolumn



% new environments and theorems
% borrowed form the LTMP-style of Springer-Verlag
% designed by Dr. Christoph Ender
%
% define the new theorem like environments
% define a new style of environments where it is possible to define
% the font % used as the theorem name and the fontfamily for the text.
%
% \newstytheorem{name}{thefont}{txtfont}{Text}[Counter]
% \newstytheorem{name}{thefont}{txtfont}[oldname]{Text}
%
% #1 - name of theorem
% #2 - theorem font
% #3 - text font
%
\def\newstytheorem#1#2#3{%
\@ifnextchar[{\che@othm{#1}{#2}{#3}}{\che@nthm{#1}{#2}{#3}}}
%
% #1 - name of theorem
% #2 - theorem font
% #3 - text font
% #4 - text string of thorem name
%
\def\che@nthm#1#2#3#4{%
\@ifnextchar[{\che@xnthm{#1}{#2}{#3}{#4}}{\che@ynthm{#1}{#2}{#3}{#4}}}
%
% #1 - name of theorem
% #2 - theorem font
% #3 - text font
% #4 - text string of thorem name
% #5 - add theorem counter to reset list of another counter
%
\def\che@xnthm#1#2#3#4[#5]{\expandafter
\@ifdefinable\csname #1\endcsname
{\@definecounter{#1}\if@envcountreset\@addtoreset{#1}{#5}\fi
\expandafter\xdef\csname the#1\endcsname{\@thmcounter{#1}}%
\global\@namedef{#1}{\che@thm{#1}{#4}{#2}{#3}}\global
\@namedef{end#1}{\@endtheorem}}}
%
% #1 - name of theorem
% #2 - theorem font
% #3 - text font
% #4 - text string of thorem name
%
\def\che@ynthm#1#2#3#4{\expandafter\@ifdefinable\csname #1\endcsname
{\@definecounter{#1}%
\expandafter\xdef\csname the#1\endcsname{\@thmcounter{#1}}%
\global\@namedef{#1}{\che@thm{#1}{#2}{#3}{#4}}\global
\@namedef{end#1}{\@endtheorem}}}
%
% change oldthm if oldname in specification is '*'
% then don't use a counter
% #1 - countername
% #2 - theorem font
% #3 - text font
% #4 - counter used
% #5 - string containing theorem name to print
%
\def\che@othm#1#2#3[#4]#5{\expandafter\@ifdefinable
\csname#1\endcsname%
{\@@othm@{#1}{#2}{#3}{#4}{#5}\global\@namedef{end#1}{\@endtheorem}}}
%
\def\@@othm@#1#2#3#4#5{\if *#4{\global\@namedef{the#1}{\relax}
\global\@namedef{#1}{\@bthm@{}{#5}{#2}{#3}}}\else
{\global\@namedef{the#1}{\@nameuse{the#4}}% define the counter
\global\@namedef{#1}{\che@thm{#4}{#5}{#2}{#3}}}\fi}
%
% new defs for theorem environment che 17-MAR-1989
% it improves the option possible
%
% #1 - countername  = theorem name
% #2 - string containing theorem name to print
% #3 - theorem font
% #4 - text font
\def\che@thm#1#2#3#4{\@ifnextchar({\@athm{#1}{#2}{#3}{#4}}{%
\@ifnextchar *{\@bthm{#1}{#2}{#3}{#4}}%
{\@@thm{#1}{#2}{#3}{#4}}}}
%
% \begin{thm}(A.1)[test]      results in:
% \end{thm}                               thm A.1 (test) ....
% #1 - theorem
% #2 - string of theorem name
% #3 - font for theorem name and label
% #4 - Text font
% #5 - is the new label
% #6 - optional string
%
\def\@athm#1#2#3#4(#5){\@ifnextchar[{%
\@aythm{#1}{#2}{#3}{#4}{#5}}{\@axthm{#1}{#2}{#3}{#4}{#5}}}
\def\@axthm#1#2#3#4#5{\@@locthmlab{#1}{#5}%
\@begintheorem{#2}{#5}{#3}{#4}\ignorespaces}
\def\@aythm#1#2#3#4#5[#6]{\@@locthmlab{#1}{#5}%
\fuh@opargbegintheorem{#2}{#5}{#6}{#3}{#4}\ignorespaces}
%
% \begin{thm}*[xytest]        results in:
% \end{thm}                               thm (xytest) ....
% #1 - theorem
% #2 - string of theorem name
% #3 - font for theorem name and label
% #4 - Text font
% #5 - optional string
%
\def\@bthm@#1#2#3#4{\@ifnextchar[{\@bythm{#1}{#2}{#3}{#4}}{%
\@bxthm{#1}{#2}{#3}{#4}}}
\def\@bthm#1#2#3#4*{\@ifnextchar[{\@bythm{#1}{#2}{#3}{#4}}{%
\@bxthm{#1}{#2}{#3}{#4}}}
\def\@bxthm#1#2#3#4{\if !#1!\relax\else\@@locthmlab{#1}{}\fi
\@@begintheorem{#2}{#3}{#4}\ignorespaces}
\def\@bythm#1#2#3#4[#5]{%
\@@opargbegintheorem{#2}{#3}{#4}{#5}\ignorespaces}
%
% define local label
%   if no number or a user specified label occured
%
\def\@@locthmlab#1#2{\expandafter\def\csname the#1\endcsname{#2}
\let\@chetempa\protect\def\protect{\noexpand\protect\noexpand}%
\edef\@currentlabel{\csname p@#1\endcsname\csname the#1\endcsname}%
\let\protect\@chetempa}
%
% #1 - counter = theorem name
% #2 - string with name
% #3 - font for theorem name and label
% #4 - font for text
\def\@@thm#1#2#3#4{\refstepcounter
    {#1}\@ifnextchar[{\che@ythm{#1}{#2}{#3}{#4}}{%
    \che@xthm{#1}{#2}{#3}{#4}}}
%
\def\che@xthm#1#2#3#4{\che@begintheorem{#2}{\csname
the#1\endcsname}{#3}{#4}\ignorespaces}
\def\che@ythm#1#2#3#4[#5]{\che@opargbegintheorem{#2}{\csname
       the#1\endcsname}{#5}{#3}{#4}\ignorespaces}
%
% #1 - name of theorem
% #2 - label string
% #3 - font for name and lable
% #4 - text fomt
%
\def\che@begintheorem#1#2#3#4{#4\trivlist\item[\hskip\labelsep
#3#1\ts #2.]}
%
% #1 - name of theorem
% #2 - label string
% #3 - additonal text
% #4 - font for name and lable
% #5 - text fomt
%
\def\che@opargbegintheorem#1#2#3#4#5{#5\trivlist
\item[\hskip\labelsep#4#1\ts #2\ #3.]}
%
\def\fuh@opargbegintheorem#1#2#3{\it \trivlist
      \item[\hskip \labelsep{\bf #1\ #2\ (#3).}]}
%
\def\@@begintheorem#1#2#3{#3\trivlist\item[\hskip\labelsep
#2#1.]}
\def\@@opargbegintheorem#1#2#3#4{#3\trivlist\item[\hskip\labelsep
#2#1 #4.]}

\newstytheorem{theorem}{\bf}{\it}{Theorem}[section]
\newstytheorem{example}{\it}{\rm}{Example}[section]
\newstytheorem{proposition}{\bf}{\it}[theorem]{Proposition}
\newstytheorem{corollary}{\bf}{\it}[theorem]{Corollary}
\newstytheorem{lemma}{\bf}{\it}[theorem]{Lemma}
\newstytheorem{proof}{\it}{\rm}[*]{Proof}
\newstytheorem{definition}{\bf}{\rm}[theorem]{Definition}
\newstytheorem{remark}{\it}{\rm}[theorem]{Remark}
\newstytheorem{exercise}{\it}{\rm}[theorem]{Exercise}
\newstytheorem{problem}{\it}{\rm}[theorem]{Problem}
\newstytheorem{solution}{\it}{\rm}[theorem]{Solution}
\newstytheorem{note}{\it}{\rm}[theorem]{Note}
\newstytheorem{question}{\it}{\rm}[theorem]{Question}

\def\squareforqed{\hbox{\rlap{$\sqcap$}$\sqcup$}}
\def\qed{\ifmmode\squareforqed\else{\unskip\nobreak\hfil
\penalty50\hskip1em\null\nobreak\hfil\squareforqed
\parfillskip=0pt\finalhyphendemerits=0\endgraf}\fi}

\def\getsto{\mathrel{\mathchoice {\vcenter{\offinterlineskip
\halign{\hfil
$\displaystyle##$\hfil\cr\gets\cr\to\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\textstyle##$\hfil\cr\gets
\cr\to\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\scriptstyle##$\hfil\cr\gets
\cr\to\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\scriptscriptstyle##$\hfil\cr
\gets\cr\to\cr}}}}}
\def\lid{\mathrel{\mathchoice {\vcenter{\offinterlineskip\halign{\hfil
$\displaystyle##$\hfil\cr<\cr\noalign{\vskip1.2pt}=\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\textstyle##$\hfil\cr<\cr
\noalign{\vskip1.2pt}=\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\scriptstyle##$\hfil\cr<\cr
\noalign{\vskip1pt}=\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\scriptscriptstyle##$\hfil\cr
<\cr
\noalign{\vskip0.9pt}=\cr}}}}}
\def\gid{\mathrel{\mathchoice {\vcenter{\offinterlineskip\halign{\hfil
$\displaystyle##$\hfil\cr>\cr\noalign{\vskip1.2pt}=\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\textstyle##$\hfil\cr>\cr
\noalign{\vskip1.2pt}=\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\scriptstyle##$\hfil\cr>\cr
\noalign{\vskip1pt}=\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\scriptscriptstyle##$\hfil\cr
>\cr
\noalign{\vskip0.9pt}=\cr}}}}}
\def\grole{\mathrel{\mathchoice {\vcenter{\offinterlineskip
\halign{\hfil
$\displaystyle##$\hfil\cr>\cr\noalign{\vskip-1pt}<\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\textstyle##$\hfil\cr
>\cr\noalign{\vskip-1pt}<\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\scriptstyle##$\hfil\cr
>\cr\noalign{\vskip-0.8pt}<\cr}}}
{\vcenter{\offinterlineskip\halign{\hfil$\scriptscriptstyle##$\hfil\cr
>\cr\noalign{\vskip-0.3pt}<\cr}}}}}
\def\bbbr{{\rm I\!R}} %reelle Zahlen
\def\bbbm{{\rm I\!M}}
\def\bbbn{{\rm I\!N}} %natuerliche Zahlen
\def\bbbf{{\rm I\!F}}
\def\bbbh{{\rm I\!H}}
\def\bbbk{{\rm I\!K}}
\def\bbbp{{\rm I\!P}}
\def\bbbone{{\mathchoice {\rm 1\mskip-4mu l} {\rm 1\mskip-4mu l}
{\rm 1\mskip-4.5mu l} {\rm 1\mskip-5mu l}}}
\def\bbbc{{\mathchoice {\setbox0=\hbox{$\displaystyle\rm C$}\hbox{\hbox
to0pt{\kern0.4\wd0\vrule height0.9\ht0\hss}\box0}}
{\setbox0=\hbox{$\textstyle\rm C$}\hbox{\hbox
to0pt{\kern0.4\wd0\vrule height0.9\ht0\hss}\box0}}
{\setbox0=\hbox{$\scriptstyle\rm C$}\hbox{\hbox
to0pt{\kern0.4\wd0\vrule height0.9\ht0\hss}\box0}}
{\setbox0=\hbox{$\scriptscriptstyle\rm C$}\hbox{\hbox
to0pt{\kern0.4\wd0\vrule height0.9\ht0\hss}\box0}}}}
\def\bbbq{{\mathchoice {\setbox0=\hbox{$\displaystyle\rm
Q$}\hbox{\raise
0.15\ht0\hbox to0pt{\kern0.4\wd0\vrule height0.8\ht0\hss}\box0}}
{\setbox0=\hbox{$\textstyle\rm Q$}\hbox{\raise
0.15\ht0\hbox to0pt{\kern0.4\wd0\vrule height0.8\ht0\hss}\box0}}
{\setbox0=\hbox{$\scriptstyle\rm Q$}\hbox{\raise
0.15\ht0\hbox to0pt{\kern0.4\wd0\vrule height0.7\ht0\hss}\box0}}
{\setbox0=\hbox{$\scriptscriptstyle\rm Q$}\hbox{\raise
0.15\ht0\hbox to0pt{\kern0.4\wd0\vrule height0.7\ht0\hss}\box0}}}}
\def\bbbt{{\mathchoice {\setbox0=\hbox{$\displaystyle\rm
T$}\hbox{\hbox to0pt{\kern0.3\wd0\vrule height0.9\ht0\hss}\box0}}
{\setbox0=\hbox{$\textstyle\rm T$}\hbox{\hbox
to0pt{\kern0.3\wd0\vrule height0.9\ht0\hss}\box0}}
{\setbox0=\hbox{$\scriptstyle\rm T$}\hbox{\hbox
to0pt{\kern0.3\wd0\vrule height0.9\ht0\hss}\box0}}
{\setbox0=\hbox{$\scriptscriptstyle\rm T$}\hbox{\hbox
to0pt{\kern0.3\wd0\vrule height0.9\ht0\hss}\box0}}}}
\def\bbbs{{\mathchoice
{\setbox0=\hbox{$\displaystyle     \rm S$}\hbox{\raise0.5\ht0\hbox
to0pt{\kern0.35\wd0\vrule height0.45\ht0\hss}\hbox
to0pt{\kern0.55\wd0\vrule height0.5\ht0\hss}\box0}}
{\setbox0=\hbox{$\textstyle        \rm S$}\hbox{\raise0.5\ht0\hbox
to0pt{\kern0.35\wd0\vrule height0.45\ht0\hss}\hbox
to0pt{\kern0.55\wd0\vrule height0.5\ht0\hss}\box0}}
{\setbox0=\hbox{$\scriptstyle      \rm S$}\hbox{\raise0.5\ht0\hbox
to0pt{\kern0.35\wd0\vrule height0.45\ht0\hss}\raise0.05\ht0\hbox
to0pt{\kern0.5\wd0\vrule height0.45\ht0\hss}\box0}}
{\setbox0=\hbox{$\scriptscriptstyle\rm S$}\hbox{\raise0.5\ht0\hbox
to0pt{\kern0.4\wd0\vrule height0.45\ht0\hss}\raise0.05\ht0\hbox
to0pt{\kern0.55\wd0\vrule height0.45\ht0\hss}\box0}}}}
\def\bbbz{{\mathchoice {\hbox{$\sf\textstyle Z\kern-0.4em Z$}}
{\hbox{$\sf\textstyle Z\kern-0.4em Z$}}
{\hbox{$\sf\scriptstyle Z\kern-0.3em Z$}}
{\hbox{$\sf\scriptscriptstyle Z\kern-0.2em Z$}}}}
\def\ts{\thinspace}

\def\typeset{\vfill\small\noindent \par}

\let\saveenddocument\enddocument
\def\enddocument{\par\typeset\saveenddocument}

%%% from scipress.sty %%%%%%%%%%%%%%%%%%%%%%
\frenchspacing
\def~{\penalty\@M\kern3pt}

\def\literaturename{References}
\def\contentsname{Table of Contents}
\def\listoffiguresname{List of Figures}
\def\figurename{Fig.\thinspace}
\def\listoftablesname{List of Tables}
\def\tablename{Table}
\def\abstractname{Abstract.}
\def\keywordname{Keywords:}

\def\thebibliography#1{\section{\literaturename}\normalsize\list
  {[\arabic{enumi}]}{\settowidth\labelwidth{#1..}\leftmargin\labelwidth
    \advance\leftmargin\labelsep
    \usecounter{enumi}}
    \def\newblock{\hskip .11em plus .33em minus -.07em}
    \sloppy
    \sfcode`\.=1000\relax}

\def\ds@citeauthoryear{\def\thebibliography##1{\section*{\literaturename}%
    \small\list{}{\settowidth\labelwidth{}\leftmargin\parindent
    \itemindent=-\parindent
    \labelsep=\z@
    \usecounter{enumi}}%
    \def\newblock{\hskip .11em plus .33em minus -.07em}%
    \sloppy
    \sfcode`\.=1000\relax}%
    \def\@cite##1{##1}%
    \def\@lbibitem[##1]##2{\item[]\if@filesw
      {\def\protect####1{\string ####1\space}\immediate
    \write\@auxout{\string\bibcite{##2}{##1}}}\fi\ignorespaces}}%


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Special Use Fonts
%%

%% Computer Modern font calls. These will be redefined
%% if PostScript fonts are used.

%% Times-Roman
\def\timesroman{cmr10 }
\def\timesbold{cmbx10 }
\def\timesitalic{cmti10 }

%% Helvetica
\def\helvetica{cmss10 }
\def\helveticabold{cmssbx10 }
\def\helveticaoblique{cmssi10 }

\def\journamefont{\footnotesize\it}
\font\runningheadfont=\helveticaoblique at 12pt
\font\foliofont=\helvetica at 12pt

\font\titlefont=\helveticabold at 14pt
\font\authorfont=\helvetica at 14pt
\font\addressfont=\helvetica at 11pt


\font\keywordnamefont=\helveticabold at 11pt
\font\keywordfont=\helvetica at 11pt

\font\abstractnamefont=\timesbold at 12pt
\let\abstractfont\normalsize

%% Section heads
\font\sectionfont=\timesbold at 12pt
\font\subsectionfont=\timesbold at 12pt

%%% <=== end special use fonts


\def\tabletitle#1{\centerline{\bf #1}\vskip4pt}

%% no widow or club lines
\widowpenalty=10000
\clubpenalty=10000

%%%%% Fleqn with mathindent set equal to parindent
\newdimen\mathindent
\mathindent=17.6pt

\renewcommand\[{\relax
                \ifmmode\@badmath
                \else
                  \begin{trivlist}%
                    \@beginparpenalty\predisplaypenalty
                    \@endparpenalty\postdisplaypenalty
                    \item[]\leavevmode
                    \hb@xt@\linewidth\bgroup $\m@th\displaystyle %$
                      \hskip\mathindent\bgroup
                \fi}
\renewcommand\]{\relax
                \ifmmode
                      \egroup $\hfil% $
                    \egroup
                  \end{trivlist}%
                \else \@badmath
                \fi}
\renewenvironment{equation}%
    {\@beginparpenalty\predisplaypenalty
     \@endparpenalty\postdisplaypenalty
     \refstepcounter{equation}%
     \trivlist \item[]\leavevmode
       \hb@xt@\linewidth\bgroup $\m@th% $
         \displaystyle
         \hskip\mathindent}%
        {$\hfil % $
         \displaywidth\linewidth\hbox{\@eqnnum}%
       \egroup
     \endtrivlist}
\renewenvironment{eqnarray}{%
    \stepcounter{equation}%
    \def\@currentlabel{\p@equation\theequation}%
    \global\@eqnswtrue\m@th
    \global\@eqcnt\z@
    \tabskip\mathindent
    \let\\=\@eqncr
    \setlength\abovedisplayskip{\topsep}%
    \ifvmode
      \addtolength\abovedisplayskip{\partopsep}%
    \fi
    \addtolength\abovedisplayskip{\parskip}%
    \setlength\belowdisplayskip{\abovedisplayskip}%
    \setlength\belowdisplayshortskip{\abovedisplayskip}%
    \setlength\abovedisplayshortskip{\abovedisplayskip}%
    $$\everycr{}\halign to\linewidth% $$
    \bgroup
      \hskip\@centering
      $\displaystyle\tabskip\z@skip{##}$\@eqnsel&%
      \global\@eqcnt\@ne \hskip \tw@\arraycolsep \hfil${##}$\hfil&%
      \global\@eqcnt\tw@ \hskip \tw@\arraycolsep
        $\displaystyle{##}$\hfil \tabskip\@centering&%
      \global\@eqcnt\thr@@
        \hb@xt@\z@\bgroup\hss##\egroup\tabskip\z@skip\cr}%
      {\@@eqncr
    \egroup
    \global\advance\c@equation\m@ne$$% $$
    \@ignoretrue
    }

\endinput
