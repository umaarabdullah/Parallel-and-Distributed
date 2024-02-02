import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.rmi.*;
import java.util.Scanner;
import java.util.stream.Collectors;

public class RMIClient
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

                    String filePath1 = "encryptedCipher.txt";
                    String filePath2 = "ceasar.txt";
                    String filePath3 = "vignerere.txt";

                    switch (userInput) {
                        case 1:
                            encrypted_message = remoteObject.requestEncryptedMessage();
                            // System.out.println("Response from Server: \n" + encrypted_message);
                            try (FileWriter writer = new FileWriter(filePath1)) {
                                writer.write(encrypted_message);
                                writer.close();
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                            break;
                        case 2:
                            if(!encrypted_message.isEmpty()){
                                decrypted_message = remoteObject.decryptWithCaesarCipher(encrypted_message, "GORDIAN");
                                // System.out.println("Decrypted using Ceasar Cipher: \n" + decrypted_message);
                                try (FileWriter writer = new FileWriter(filePath2)) {
                                    writer.write(decrypted_message);
                                    writer.close();
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
                            }
                            else
                                System.out.println("Encrypted text not available. Request from server !");
                            break;
                        case 3:
                            if(!encrypted_message.isEmpty()){
                                decrypted_message = remoteObject.decryptWithVigenereCipher(encrypted_message, "GORDIAN");
                                // System.out.println("Decrypted using Vigenere Cipher: \n" + decrypted_message);
                                try (FileWriter writer = new FileWriter(filePath3)) {
                                    writer.write(decrypted_message);
                                    writer.close();
                                } catch (IOException e) {
                                    e.printStackTrace();
                                }
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