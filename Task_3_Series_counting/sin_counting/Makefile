JOB_NAME = Sin
JOB_SH = job.sh
EXEC = sin
SRC = sin.c

all:   
	@ mpicc ${SRC} -o ${EXEC} -lm
	@ qsub ${JOB_SH}
clean: 
	@ rm ${JOB_NAME}.* ${EXEC}

