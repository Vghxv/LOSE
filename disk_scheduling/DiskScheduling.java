import java.util.*;

public class Disk_Scheduling {
    private static final int MAX_CYLINDER = 5000;
    private static final int NUM_REQUESTS = 1000;

    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java DiskScheduling <initial head position>");
            return;
        }

        int initialPosition = Integer.parseInt(args[0]);
        int[] requests = generateRequests(NUM_REQUESTS, MAX_CYLINDER);
        printRequests(requests);

        System.out.println("Initial head position: " + initialPosition);
        System.out.println("Total head movement (FCFS): " + fcfs(requests, initialPosition));
        System.out.println("Total head movement (SCAN): " + scan(requests, initialPosition, 5000));
        System.out.println("Total head movement (C-SCAN): " + cscan(requests, initialPosition, 5000));
    }

    private static int[] generateRequests(int numRequests, int maxCylinder) {
        Random rand = new Random();
        int[] requests = new int[numRequests];
        for (int i = 0; i < numRequests; i++) {
            requests[i] = rand.nextInt(maxCylinder);
        }
        return requests;
    }

    private static void printRequests(int[] requests) {
        for (int request : requests) {
            System.out.print(request + " ");
        }
        System.out.println();
    }

    private static int fcfs(int[] requests, int initialPosition) {
        int totalMovement = 0;
        int currentPosition = initialPosition;

        for (int request : requests) {
            totalMovement += Math.abs(currentPosition - request);
            currentPosition = request;
        }

        return totalMovement;
    }

    private static int scan(int[] requests, int initialPosition, int maxCylinder) {
        Arrays.sort(requests);
        int totalMovement = 0;
        int currentPosition = initialPosition;

        for (int request : requests) {
            if (request >= initialPosition) {
                totalMovement += Math.abs(currentPosition - request);
                currentPosition = request;
            }
        }

        if (currentPosition < maxCylinder - 1) {
            totalMovement += Math.abs(currentPosition - (maxCylinder - 1));
            currentPosition = maxCylinder - 1;
        }

        for (int i = requests.length - 1; i >= 0; i--) {
            if (requests[i] < initialPosition) {
                totalMovement += Math.abs(currentPosition - requests[i]);
                currentPosition = requests[i];
            }
        }

        return totalMovement;
    }

    private static int cscan(int[] requests, int initialPosition, int maxCylinder) {
        Arrays.sort(requests);
        int totalMovement = 0;
        int currentPosition = initialPosition;

        for (int request : requests) {
            if (request >= initialPosition) {
                totalMovement += Math.abs(currentPosition - request);
                currentPosition = request;
            }
        }

        if (currentPosition < maxCylinder - 1) {
            totalMovement += Math.abs(currentPosition - (maxCylinder - 1));
            totalMovement += (maxCylinder - 1); // to simulate the jump back to the start (0)
            currentPosition = 0;
        }

        for (int request : requests) {
            if (request < initialPosition) {
                totalMovement += Math.abs(currentPosition - request);
                currentPosition = request;
            }
        }

        return totalMovement;
    }
}
