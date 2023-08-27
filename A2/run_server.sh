sudo service rpcbind start

# give execute permission
chmod +x run_add.sh run_mul.sh run_inverse.sh run_transpose.sh;

# Run server
cmd.exe /c start cmd.exe /c wsl.exe -- bash -c "cd Matrix_Op_RPC; chmod +x matrixOp_server matrixOp_client; make -f Makefile.matrixOp; ./matrixOp_server"
# gnome-terminal -- bash -c "cd Matrix_Op_RPC; chmod +x matrixOp_server matrixOp_client; make -f Makefile.matrixOp; ./matrixOp_server"
