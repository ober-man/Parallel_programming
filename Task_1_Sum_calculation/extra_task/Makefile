JOB_NAME = Sum_extra
JOB_SH = job.sh
EXEC = sum_extra
SRC = sum_extra.c

all:    
	@ mpicc ${SRC} -o ${EXEC}
	@ qsub ${JOB_SH}
clean:
	@ rm ${JOB_NAME}.* ${EXEC}

