JOB_NAME = Sum
JOB_SH = job.sh
EXEC = sum
SRC = sum.c

all:   
	@ mpicc ${SRC} -o ${EXEC}
	@ qsub ${JOB_SH}
clean: 
	@ rm ${JOB_NAME}.* ${EXEC}

