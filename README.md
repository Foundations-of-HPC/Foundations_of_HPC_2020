# Foundations of HPC, 2020/2021
This repository collects the materials from the course "Foundations of HPC" at Data Science and Scientific Computer, University of Trieste, year 2020/2021

Teachers:

**Stefano Cozzini**, Area di Ricerca, Trieste + ExactLab, Trieste

**Luca Tornatore**, INAF - National Institute for Astrophysics, Astronomical Observatory of Trieste

Lab session are helped by:
**Dott. Antonio Ragagnin**, INAF - National Institute for Astrophysics, Astronomical Observatory of Trieste

------

#### -- DSSC -- Data Science and Scientific Computing @ University of Trieste

#### -- Master in HPC @ SISSA/ICTP, Trieste

------



Welcome to the "Foundations of HPC" course at *(i)* DSSC Department of the University of Trieste and *(ii)* Master in HPC program at SISSA/ICTP in Trieste.

The scope of the course is to give you:

- an introduction to basic concepts of the High Performance Computing
- an introduction to the modern computer architecture and the techniques to exploit it to maximize performance
- an introduction to parallel/multi-threading programming with MPI and OpenMP
- an introduction to basic and advanced techniques for code optimisation for single- and multi-threaded codes



The lectures will be both in presence (this may change in the future according to the evolution of the COVID pandemic) **and** in streaming; each lecture will also be recorded and available to be viewed asynchronously.
The lectures (the precise calendar is shown in the table below) will be held in AULA BOH on **Wednesday afternoon** from 14:00 to 16:00, and **Friday morning** from 11:00 to 13:00.
On **Friday afternoon**, from 14:00 to 16:00, there will be a lab session with exercises on the topics treated during the lectures.

The lectures will start on Wednesday, 7th October 2020


|           | 9.00 - 9.55  | 10.00 - 11.00 | | 14.00 - 14.55 | 15.00 - 16.00 |
| --------- | ---------- | ----------- | - |----------- | ----------- |
| Wednesday |            |             | | first part  | second part |
| Friday    | first part | second part | | lab         | lab         |



###### Contact details:

Stefano Cozzini:  stefano.cozzini@areasciencepark.it

Luca Tornatore:  luca.tornatore@inaf.it 

The lab sessions will be assisted by Dott. Antonio Ragagnin, antonio.ragagnin@inaf.it



#### EVALUATION

The final grade of the exam will be the result of a written and an oral examination.

During the course, the students will be asked to carry out 7 assignments. It will be required to submit the code developed and the results written in a given format (so to allow automatic checking) on a repo that we will indicate. Each assignment will have a due date of either 1 or 2 weeks. 

If the first 5 assignments will have been submitted and will be successful, the student will be directly admitted to a final oral examination. The oral examination will focus on the topics covered in the lessons, and it will start from the examination of some of the assignments.

If the either the first 5 assignments will not have been submitted, or they are not successful, a written exam will be required. This test will consist in a project that must be submitted at least 1 week before taking the oral examination.

For submitting 7 *successful* assignments in the due time before the end of the course, a +1 will be rewarded in the final mark.




#### CALENDAR

| DATE        | LECTURES                                                     | TUTORIALS                                            | ASSIGMENTS                                           |
| :---------- | :----------------------------------------------------------- | :--------------------------------------------------- | :--------------------------------------------------- |
| Wed, Oct 7  | [1] Introduction to course/ introduction to HPC              |                                                      |                                                      |
| Fri, Oct 9  | [2] Introduction to HPC* / 2                                 |                                                      |                                                      |
| Wed, Oct 14 | [3] HPC Hardware and parallel computing                      |                                                      |                                                      |
| Fri, Oct 16 | [4] HPC Software stack and tools                             | [1] Using an HPC infrastructure                      | [1] Assigment on introductory topics  *(1 week)*     |
| Wed, Oct 21 | [5] Single Core Optimization (SCO) / 1<br />*Modern Architecture + Opt. Intro + Stack & Heap + Memory allocation* |                                                      |                                                      |
| Fri, Oct 23 | [6] SCO / 2<br />*How to use the Compiler + Cache hierarchy* | [2] Exercises on SCO                                 |                                                      |
| Wed, Oct 28 | [7] SCO / 3<br />*loops*                                     |                                                      |                                                      |
| Fri, Oct 30 | [8] SCO / 4<br />*Pipelines +branch predictions*             | [3] Tools for profiling                              |                                                      |
| Wed, Nov 4  | [9] SCO / 5<br />*Prefetching + vectorization*               |                                                      |                                                      |
| Fri, Nov 6  | [10] SCO / 6<br />*Debugging*                                | [4] Exercises on SCO                                 |                                                      |
| Wed, Nov 11 | [11] MPI / 1                                                 |                                                      |                                                      |
| Fri, Nov 13 | [12] MPI / 2                                                 | [4] Exercises on MPI                                 | [2] Assigment on MPI  *(1 week)*                     |
| Wed, Nov 18 | [13] OpenMP / 1                                              |                                                      |                                                      |
| Fri, Nov 20 | [14] OpenMP / 2                                              | [5] Exercises on OpenMP                              | [3] Assigment on OpenMP  *(1 week)*                  |
| Wed, Nov 25 | [15] MPI / 3                                                 |                                                      |                                                      |
| Fri, Nov 27 | [16] MPI / 4                                                 | [6] Exercises on MPI                                 |                                                      |
| Wed, Dec 2  | [17] OpenMP / 3                                              |                                                      |                                                      |
| Fri, Dec 4  | [18] OpenMP / 4                                              | [7] Exercises on OpenMP                              |                                                      |
| Wed, Dec 9  | [19] MPI / 5                                                 |                                                      |                                                      |
| Fri, Dec 11 | [20] OpenMP / 5                                              | [8] Exercises on MPI and OpenMP                      | [4] + [5] Assigments on MPI and OpenMP  *(2 weeks)*  |
| Wed, Dec 16 | [21] Parallel HPC libraries / 1                              |                                                      |                                                      |
| Fri, Dec 18 | [22] Parallel HPC libraries / 2 + benchmarking               | [9] Exercises on benchmarking HPC parallel libraries | [6] Assignment on Parallel HPC libraries  *(1 week)* |
| Fri, Jan 8  | [23] Parallel Storage / 2                                    |                                                      |                                                      |
| Wed, Jan 13 | [24] Parallel Storage / 2                                    |                                                      |                                                      |
| Fri, Jan 15 | [25] roof model + parallel performance analysis              | [10] Exercises                                       | [7] Assignment on Parallel Storage  *(1 week)*       |
| Wed, Jan 20 | [26] TBD - advanced materials                                |                                                      |                                                      |
| Fri, Jan 22 | [27] TBD - advanced materials                                |                                                      |                                                      |
| Wed, Jan 27 | [28] TBD - advanced materials                                |                                                      |                                                      |
| Fri, Jan 29 | [29] TBD - advanced materials                                |                                                      |                                                      |



#### REFERENCES & BOOKS

##### Ref 1: High Performance Computing Modern Systems and Practices
Available at ICTP library Authors: Thomas Sterling Matthew Anderson Maciej Brodowicz eBook ISBN: 9780124202153 Paperback ISBN: 9780124201583

Some of the materials of this book are presented during some lectures.

##### Ref 2: Introduction to High-Performance Scientific Computing,by Victor Eijkhout
The source and pdf of the book (as well as lecture slides) can be found in this repository: https://bitbucket.org/VictorEijkhout/hpc-book-and-course 10.5281/zenodo.49897

Some of the materials presented in classes are taken from this book.

##### Ref 3: Computer Organization and Design
by D. A. Patterson and J. L. Hennessy The Morgan Kaufmann Series in Computer Architecture and Design easily available as pdf on the net.

##### Ref 4: Introduction to High Performance Computing for Scientists and Engineers
by Georg Hager and Gerhard Wellein Paperback: 356 pages Publication date July 2, 2010 Editors (Chapman & Hall/CRC Computational Science)

##### Ref 5: Optimizing HPC Applications with Intel Cluster Tools
Paperback – October 15, 2014 by Alexander Supalov (Author), Andrey Semin (Author), Michael Klemm (Author), & 1 more ISBN-13: 978-1430264965 ISBN-10: 1430264969 Edition: 1st

##### Ref 6: High Performance Parallelism Pearls, 1st Edition
Multicore and Many-core Programming Approaches by Reinders & Jeffers
Morgan Kaufmann, Print Book ISBN :9780128021187
