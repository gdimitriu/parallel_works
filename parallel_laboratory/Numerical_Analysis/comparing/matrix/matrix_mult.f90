	PROGRAM matrix
	REAL(4) VAL(2)
	PARAMETER DIM=1000,NUMAR=10
	DOUBLE PRECISION MATA(DIM,DIM),MATB(DIM,DIM),MATC(DIM,DIM)
	REAL(4) TARRAY(2)
	DO I=1,DIM
	DO J=1,DIM
		MATA(I,J)=DFLOATJ(IRAND())
		MATB(I,J)=DFLOATJ(IRAND())
	END DO
	END DO
	DO L=1,NUMAR
		VAL(1)=DTIME(TARRAY)
		DO I=1,DIM
		DO J=1,DIM
			MATC(I,J)=0.0
		DO K=1,DIM
			MATC(I,J)=MATC(I,J)+MATA(I,K)*MATB(K,J)
		END DO
		END DO
		END DO
		VAL(2)=DTIME(TARRAY)
		PRINT *,TARRAY(1),'AND',TARRAY(2),'OR',VAL(2)
	END DO
	END
	