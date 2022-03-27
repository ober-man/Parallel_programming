JOB_NAME = Hello_World
JOB_SH = job.sh
EXEC = helloworld
SRC = helloworld.c

all:	
	@ mpicc ${SRC} -o ${EXEC}
	@ qsub ${JOB_SH}
clean:
	@ rm ${JOB_NAME}.* ${EXEC}
