! Program pentru calculul integralei cu metoda trapezului
	PROGRAM TRAPEZ
	DOUBLE PRECISION LS,LD,H,INTEGRALE
	INTEGER(4) N
	READ *,LS,LD,N
	H=(LD-LS)/N
	INTEGRALE=H*(FUNC(LS)+FUNC(LD))/2
	DO I=1,N-1
		INTEGRALE=INTEGRALE+H*FUNC(LS+I*H)
	END DO
	PRINT *,'INTEGRALA ESTE ',INTEGRALE
	END
	DOUBLE PRECISION FUNCTION FUNC(X)
		DOUBLE PRECISION X
		FUNC=SIN(X)
	END FUNCTION FUNC
	