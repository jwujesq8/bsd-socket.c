import java.net.*;
import java.io.*;

public class Client {
    private Socket socket;
    private InetAddress adresSerwera; //serverAddress
    private int portSerwera;
    private ObjectInputStream ois;
    private ObjectOutputStream oos;

    public Client(InetAddress address, int port) {
        super();
        serverAddress = address;
        serverPort = port;
    }

    public void openConnection() {
        try {
            System.out.println("Nawiazuje polaczenie (connecting)...");
            socket = new Socket( serverAddress, serverPort);
            oos = new ObjectOutputStream(s.getOutputStream());
            ois = new ObjectInputStream(s.getInputStream());
            System.out.println("Polaczenie nawiazane (connection established).");
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    public void ZamknijPolaczenie() { //CloseConnection
        try {
            System.out.println("Zamykam polaczenie (closing connection)...");
            oos.close();
            ois.close();
            socket.close();
            System.out.println("Polaczenie zamkniete (connection closed).");
        } catch(Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    public void stringToPDF(){
        Document document = new Document();
        PdfWriter.getInstance(document, new FileOutputStream("iTextHelloWorld.pdf"));

        document.open();
        Font font = FontFactory.getFont(FontFactory.COURIER, 16, BaseColor.BLACK);
        Chunk chunk = new Chunk("Hello World", font);

        document.add(chunk);
        document.close();
    }
    public Object executeTaskOnServer(
            String  = null;
        try {
        oos.writeObject(z);
        oos.writeObject(par);
        wynik = ois.readObject();
    } catch(Exception e) {
        e.printStackTrace();
        System.exit(1);
    }
        return wynik;

    }

    public void Menu() {
        while (true) {

            System.out.println("");
            try {
                BufferedReader klawiatura = new BufferedReader(
                    new InputStreamReader(
                        System.in
                        )
                    );
                System.out.print("Wybor (choice): ");
                int opcja = Integer.parseInt(
                    klawiatura.readLine());
                if (opcja == 3) System.exit(0);
                if (opcja == 1 || opcja == 2) {
                    OtworzPolaczenie();
                    if (opcja == 1) {
                        Double wynik = (Double)
                            WykonajZadanieNaSerwerze(
                                zadanie1,
                                new Double(0.0)
                            );
                        System.out.println(
                        "Wynik zadania (result of task) 1: "+wynik.doubleValue());
                    } else {
                        int[] wynik = (int[])
                            WykonajZadanieNaSerwerze(
                                zadanie2,
                                tablica
                            );
                        System.out.print("Wynik zadania (result of task) 2: [ ");
                        for (int i = 0; i < wynik.length; i++) {
                            System.out.print(wynik[i]+" ");
                        }
                        System.out.println("]");
                    }
                    ZamknijPolaczenie();
                }
            } catch (Exception e) {
                System.out.println("NIE POWIODLO SIE (failed)");
            }
        }
    }

    public static void main(String[] args) {
        String host;
        int port;
        try {
            BufferedReader br = new BufferedReader(
                new InputStreamReader(
                    System.in
                    )
                );
            System.out.print("Host serwera obliczen (address of th computation server): ");
            host = br.readLine();
            System.out.print("Numer portu serwera obliczen (port number of the server): ");
            port = Integer.parseInt(br.readLine());

            Client client = new Client( InetAddress.getByName(host), port);
            openConnection();
            c.Menu();
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }
}
