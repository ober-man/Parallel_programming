JOB_NAME = Pi
JOB_SH = job.sh
EXEC = pi
SRC = pi.c

all:   
	@ mpicc ${SRC} -o ${EXEC} -lm
	@ qsub ${JOB_SH}
clean: 
	@ rm ${JOB_NAME}.* ${EXEC}

