JOB_NAME = Circle
JOB_SH = job.sh
EXEC = circle
SRC = circle.c

all:   
	@ mpicc ${SRC} -o ${EXEC}
	@ qsub ${JOB_SH}
clean: 
	@ rm ${JOB_NAME}.* ${EXEC}

