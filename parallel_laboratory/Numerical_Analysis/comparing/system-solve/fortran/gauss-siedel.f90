	PROGRAM GAUSS_SIEDEL
	PARAMETER DIM=1000,NUMAR=10,DIM1=DIM+1,EROARE=1E-25
	REAL(4) VAL(2)
	REAL(4) TARRAY(2)
	DOUBLE PRECISION MAT(DIM,DIM1),X(DIM),XOLD
	DOUBLE PRECISION TEMP
	DOUBLE PRECISION REZ(DIM)
	INTEGER FLAG
	DO I=1,DIM
		REZ(I)=I
	END DO
	DO L=1,NUMAR
		DO I=1,DIM
			DO J=1,DIM
				MAT(I,J)=RANF()
			END DO
			TEMP=0.0
			DO J=1,DIM
				IF(J .NE. I) THEN
					TEMP=TEMP+ABS(MAT(I,J))
				END IF
			END DO
			MAT(I,I)=TEMP+MAT(I,I)
		END DO
		DO I=1,DIM
			MAT(I,DIM1)=0.0
			DO J=1,DIM
				MAT(I,DIM1)=MAT(I,DIM1)+MAT(I,J)*REZ(J)
			END DO
		END DO
		DO I=1,DIM
			TEMP=0.0
			DO J=1,DIM
				IF(J .NE. I) THEN
					TEMP=TEMP+ABS(MAT(I,J)/MAT(I,I))
				END IF
			END DO
			IF (TEMP .GE. 1.0) THEN
				PRINT *,'GAUSS-SIEDEL NU CONVERGE'
				GO TO 999
			END IF
		END DO
		VAL(1)=DTIME(TARRAY)
		DO I=1,DIM
			X(I)=MAT(I,DIM1)
		END DO
10		FLAG=0
		DO I=1,DIM
			TEMP=0.0
			DO J=1,I-1
				TEMP=TEMP+MAT(I,J)*X(J)
			END DO
			DO J=I+1,DIM
				TEMP=TEMP+MAT(I,J)*X(J)
			END DO
			XOLD=X(I)
			X(I)=(MAT(I,DIM1)-TEMP)/MAT(I,I)
			IF( ABS(X(I)-XOLD) .GT.  EROARE) THEN
				FLAG=1
			END  IF
		END DO
		IF (FLAG .EQ. 1) THEN
			GO TO 10
		END IF
		VAL(2)=DTIME(TARRAY)
		PRINT *,TARRAY(1),"AND",TARRAY(2),"OR",VAL(2)
!		DO I=1,DIM
!			PRINT *,REZ(I),'=',X(I)
!		END DO
	END DO
999	END
	