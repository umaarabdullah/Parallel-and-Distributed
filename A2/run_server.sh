sudo service rpcbind start

# give execute permission
chmod +x run_client1.sh run_client2.sh;

# Run server
cmd.exe /c start cmd.exe /c wsl.exe -- bash -c "cd Matrix_Op_RPC; chmod +x matrixOp_server matrixOp_client; make -f Makefile.matrixOp; ./matrixOp_server;"

# gnome-terminal -- bash -c "cd Matrix_Op_RPC; chmod +x matrixOp_server matrixOp_client; make -f Makefile.matrixOp; ./matrixOp_server"
