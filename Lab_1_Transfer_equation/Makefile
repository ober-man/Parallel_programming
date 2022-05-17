JOB_NAME = Diffur
JOB_SH = job.sh
EXEC = diffur
SRC = diffur.c

all:   
	@ mpicc ${SRC} -o ${EXEC}
	@ qsub ${JOB_SH}
clean: 
	@ rm ${JOB_NAME}.* ${EXEC}

