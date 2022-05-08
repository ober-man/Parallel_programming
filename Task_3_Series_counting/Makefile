JOB_NAME = Exp
JOB_SH = job.sh
EXEC = exp
SRC = exp.c

all:   
	@ mpicc ${SRC} -o ${EXEC}
	@ qsub ${JOB_SH}
clean: 
	@ rm ${JOB_NAME}.* ${EXEC}

