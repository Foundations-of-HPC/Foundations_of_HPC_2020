 
#PBS -l nodes=1:ppn=4 
#PBS -l walltime=00:10:00
#PBS -q fat

pwd 
cd $PBS_O_WORKDIR
pwd

./omp101

export OMP_NUM_THREADS=20
./omp101
exit

