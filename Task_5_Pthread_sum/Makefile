JOB_NAME = Sum
JOB_SH = job.sh
EXEC = sum
SRC = sum_thr.c

all:   
	@ gcc ${SRC} -o ${EXEC} -lpthread -lrt

clean: 
	@ rm ${JOB_NAME}.* ${EXEC}

