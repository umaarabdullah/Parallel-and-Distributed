// Java program for server application
import java.rmi.*;
import java.rmi.registry.*;
public class RMIServer
{
    // Creating Stub and Skeleton objects from the implementation class using rmic 
	public static void main(String args[])
	{
		System.out.println("RMI Server running.......");

		try
		{
			// Create an object of the interface
			// implementation class
            RMIServerInterface server = new RMIServerImpl();

			// rmiregistry within the server JVM with
			// port number 1900
			LocateRegistry.createRegistry(1900);

			// Binds the remote object by the name
			// geeksforgeeks
			Naming.rebind("rmi://localhost:1900"+
						"/umaar_server",server);
		}
		catch(Exception ae)
		{
			System.out.println(ae);
		}
	}
}
