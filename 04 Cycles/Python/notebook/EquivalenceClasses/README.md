## Python Skript for Analysing Equivalence Classes

### 1. Abstract
The following code provides my analaysis of Equivalence Class

#### A rough block diagram
![image](https://user-images.githubusercontent.com/66524778/131253715-6b8d0e5b-c6ca-4bbc-bddc-3c71e1833c7d.png)

### 2. Gliederung
The JupyterNotebook is structured as follow:
* Field 1: Libraries
* Field 2: Functions for preparing Dataframe
* Field 3: Prepare Dataframe
* Field 4: Prepare Equivalence Class
* Field 5: Get Dataframe
* Field 6: Show Equivalence Class
* Field 7: Prepare Cardinality Dataframe
* Field 8: Show Cardinalities for all Hammingweights
* Field 9: Number of Elements per Equivalence Class
### 3. Walktrough
1. Run all Fields in chronological order (from 1 to 9). From now on you can load each field individually for later analysis
2. Find our Inputvariables
    * (Field 5: ln 3) Variable n to set length of Binaryword
    * (Field 5: ln 9) Change Dataframe to [1] or [0] to *show* different Inputtables
    * (Field 6: ln 1) Index (=line in Dataframe[1]) to show Equivalence Class for any choosen root (see below)
    * (Field 9: ln 3) Hammingweight t to get all Equivalence Classes for given n, t
3. Try two diffent Inputvariabels and rerun fields (5 - 9) to get familiar with the Outputs
    * completely trivial cases: e.g. n = 5 with any Hammingweight t
    * Non trival cases: e.g. n = 6 with trival cases for t = 1,2,5,6 and Non trivial ones for t = 3,4
4. Understand the Outputs
    * (Field 5) Returns our Inputdataframe (If [0]: all possible Binarywords, or [1]: only all possible roots)
    * (Field 6) Shows Equivalence Class for any choosen root (by index = row in Dataframe[1])
    * (Field 7) Prepars Cardinality Dataframe (Must be reruned by any variable change before runing Field 8 and 9!)
    * (Field 8) Returns all Cardinalities of all Binarywords with given length n (Sorted by Hammingweight)
    * (Field 9) 1. Returns all Cardinalitys D ={d1,d2,..} for given length n and given Hammingweight t (Each Cardinality results in one Equivalence Class)
    * (Field 9) 2. Number of Elements per Equivalence Class C(n,t,d) for each Cardinality d

#### For any questions please contact me adrian@abold-family.de, 0151/56588263
