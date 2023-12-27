import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMIServerInterface extends Remote {
    // Method to request the encrypted message from the server
    String requestEncryptedMessage() throws RemoteException;

    // Method to request the Vigenere cipher table (Tabula Recta) from the server
    String requestVigenereCipherTable() throws RemoteException;

    // Method to perform decryption using Caesar cipher
    String decryptWithCaesarCipher(String encryptedMessage, String keyword) throws RemoteException;

    // Method to perform decryption using Vigenere cipher
    String decryptWithVigenereCipher(String encryptedMessage, String keyword) throws RemoteException;
}
