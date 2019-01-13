! BSD 2-Clause License
! 
! Copyright (c) 2019, Oscar Riveros - www.peqnp.science.
! All rights reserved.
! 
! Redistribution and use in source and binary forms, with or without
! modification, are permitted provided that the following conditions are met:
! 
! 1. Redistributions of source code must retain the above copyright notice, this
!    list of conditions and the following disclaimer.
! 
! 2. Redistributions in binary form must reproduce the above copyright notice,
!    this list of conditions and the following disclaimer in the documentation
!    and/or other materials provided with the distribution.
! 
! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
! AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
! IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
! DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
! FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
! DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
! SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
! CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
! OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
! OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

include 'io.f08'
include 'utils.f08'

program program
    use io
    use utils

    implicit none

    integer :: i, j, n
    real :: partial_sum, sum
    real, allocatable :: matrix(:, :)[:]

    open(unit = 1 , file = 'matrix.txt')
    read (1, *) n
    allocate(matrix(n, n)[*])
    read (1, *) matrix
    matrix = transpose(matrix)
    close(1)

    call print_matrix(matrix, this_image(), num_images())

    sync images(*)

    if (this_image() == 1) then    
        sum = 0
        do i = 1, num_images()
            partial_sum = sum_matrix(matrix, i, num_images())   
            sum = sum + partial_sum
            write (*, "(a6,i4.2,a22,f8.2)") 'image:', i, '  partial_sum(matrix) =', partial_sum       
        end do
        write (*, *) 'sum(matrix) =', sum         
    end if 
    
    deallocate(matrix)

end program program
