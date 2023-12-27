import java.rmi.*;
import java.util.Scanner;

public class RMIClient1
{
    public static void main(String args[])
    {
        String encrypted_message = "";
        String decrypted_message = "";

        try
        {
            // lookup method to find reference of remote object
            RMIServerInterface remoteObject =
                (RMIServerInterface)Naming.lookup("rmi://localhost:1900"+
                                    "/umaar_server");
            System.out.println("Remote lookup in Progress"); 

            try (Scanner scanner = new Scanner(System.in)) {
                
                while(true){

                    System.out.println("-----------Choose an option:-------------");
                    System.out.println("--------1. Request Encrypted text--------");
                    System.out.println("--------2. Decrypt using Caesar cipher---");
                    System.out.println("--------3. Decrypt using Vigenère cipher-");

                    int userInput = scanner.nextInt();
                    scanner.nextLine(); // Consume the newline character

                    switch (userInput) {
                        case 1:
                            encrypted_message = remoteObject.requestEncryptedMessage();
                            System.out.println("Response from Server: \n" + encrypted_message);
                            break;
                        case 2:
                            if(!encrypted_message.isEmpty()){
                                decrypted_message = remoteObject.decryptWithCaesarCipher(encrypted_message, "GORDIAN");
                                System.out.println("Decrypted using Ceasar Cipher: \n" + decrypted_message);
                            }
                            else
                                System.out.println("Encrypted text not available. Request from server !");
                            break;
                        case 3:
                            if(!encrypted_message.isEmpty()){
                                decrypted_message = remoteObject.decryptWithVigenereCipher(encrypted_message, "GORDIAN");
                                System.out.println("Decrypted using Vigenere Cipher: \n" + decrypted_message);
                            }
                            else
                                System.out.println("Encrypted text not available. Request from server !");
                            break;
                    }
                }
            }
            

        }
        catch(Exception ae)
        {
            System.out.println(ae);
        }
    }
}