	PROGRAM teste
	PARAMETER (n=3)
	REAL A(n,n),b(n),y(n),x(n)
	A(1,1)=1
	A(1,2)=1
	A(1,3)=1
	A(2,1)=1
	A(2,2)=-1
	A(2,3)=-1
	A(3,1)=1
	A(3,2)=-1
	A(3,3)=1
	b(1)=6
	b(2)=-4
	b(3)=2
	call gauss(A,b,y,n)
	call back(A,x,y,n)
	do i=1,n,1
		write(*,*) 'x(',i,')=',x(i)
	enddo
	do i=1,n,1
		do j=1,n,1
			write(*,*) A(i,j)
		enddo
	enddo
	end
	SUBROUTINE gauss(A,b,y,n)
	INTEGER n,i,j,k
	REAL A(n,n),b(n),y(n)
	do k=1,n,1
		do j=k+1,n,1
			A(k,j)=a(k,j)/a(k,k)
		enddo
		y(k)=b(k)/A(k,k)
		a(k,k)=1
		do i=k+1,n,1
			do j=k+1,n,1
				A(i,j)=A(i,j)-A(i,k)*A(k,j)
			enddo
			b(i)=b(i)-A(i,k)*y(k)
			A(i,k)=0
		enddo
	enddo
	return
	end
	SUBROUTINE back(A,x,y,n)
	INTEGER n,i,j,k
	REAL A(n,n),x(n),y(n)
	do k=n,1,-1
		x(k)=y(k)
		do i=k,1,-1
			y(i)=y(i)-x(k)*A(i,k)
		enddo
	enddo
	return
	end
