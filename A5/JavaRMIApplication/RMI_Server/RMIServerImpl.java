import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.stream.Collectors;

public class RMIServerImpl extends UnicastRemoteObject implements RMIServerInterface {
    private static final long serialVersionUID = 1L;

    // Constructor for the server implementation
    protected RMIServerImpl() throws RemoteException {
        super();
    }

    // Method to read the encrypted message from the hosted file
    private String readEncryptedMessageFromFile() {
        // Logic to read the encrypted message from the file and return it as a String

        String filePath = "files/Cipher.txt";
        String encryptedMessage = "";
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            encryptedMessage = reader.lines().collect(Collectors.joining(System.lineSeparator()));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return encryptedMessage;
    }

    // Method to generate the Vigenere cipher table (Tabula Recta)
    private String generateVigenereCipherTable() {
        // Logic to generate the Vigenere cipher table (Tabula Recta)
        // Return the generated table as a String

        String filePath = "files/TabulaRecta.txt";
        String tabulaRecta = "";
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            tabulaRecta = reader.lines().collect(Collectors.joining(System.lineSeparator()));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return tabulaRecta;
    }

    // Method to perform decryption using Caesar cipher
    private String decryptWithCaesarCipher_Server(String encryptedMessage, String keyword) {
        // Logic to decrypt using Caesar cipher with the provided keyword
        
        StringBuilder decrypted = new StringBuilder();
        int keywordIndex = 0;

        for (char letter : encryptedMessage.toCharArray()) {
            if (Character.isLetter(letter)) {
                char base = Character.isUpperCase(letter) ? 'A' : 'a';
                int shift = keyword.charAt(keywordIndex % keyword.length()) - 'A';
                int decryptedChar = (letter - base - shift + 26) % 26 + base;
                decrypted.append((char) decryptedChar);
                keywordIndex++;
            } else {
                decrypted.append(letter);
            }
        }
        return decrypted.toString();
    }

    // Method to perform decryption using Vigenere cipher
    private String decryptWithVigenereCipher_Server(String encryptedMessage, String keyword) {
        // Logic to decrypt using Vigenere cipher with the provided keyword
        
        StringBuilder decrypted = new StringBuilder();
        int keywordIndex = 0;

        for (char letter : encryptedMessage.toCharArray()) {
            if (Character.isLetter(letter)) {
                char base = Character.isUpperCase(letter) ? 'A' : 'a';
                int key = keyword.charAt(keywordIndex % keyword.length()) - base;
                int shift = (base - 'A' + 26 - key) % 26;
                char decryptedChar = (char) (base + (letter - base + shift) % 26);
                decrypted.append(decryptedChar);
                keywordIndex++;
            } else {
                decrypted.append(letter);
            }
        }
        return decrypted.toString();
    }

    // Method to request the encrypted message from the server
    public String requestEncryptedMessage() throws RemoteException {
        return readEncryptedMessageFromFile();
    }

    // Method to request the Vigenere cipher table from the server
    public String requestVigenereCipherTable() throws RemoteException {
        return generateVigenereCipherTable();
    }

    // Method to perform decryption using Caesar cipher
    public String decryptWithCaesarCipher(String encryptedMessage, String keyword) throws RemoteException {
        return decryptWithCaesarCipher_Server(encryptedMessage, keyword);
    }

    // Method to perform decryption using Vigenere cipher
    public String decryptWithVigenereCipher(String encryptedMessage, String keyword) throws RemoteException {
        return decryptWithVigenereCipher_Server(encryptedMessage, keyword);
    }
}
