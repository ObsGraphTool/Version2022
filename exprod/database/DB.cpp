states
inactive
waiting
processing
sent
acks
transitions
(inactive, sendmess(I) ,waiting)
(waiting, recvack(I) ,inactive)
(sent, recvmess(I) ,processing)
(processing, sendack(I) ,acks)
