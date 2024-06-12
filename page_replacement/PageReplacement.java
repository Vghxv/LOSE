import java.util.*;

public class PageReplacement {
    private int[] referenceString;
    private int pageFrame;
    private int referenceStringLength;

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("Usage: java PageReplacement <pageFrame> <referenceStringLength>");
            System.exit(1);
        }

        int pageFrame = Integer.parseInt(args[0]);
        int referenceStringLength = Integer.parseInt(args[1]);

        PageReplacement pr = new PageReplacement(pageFrame, referenceStringLength);
        pr.generateReferenceString();
        pr.printReferenceString();
        pr.fifo();
        pr.lru();
        pr.opt();
    }

    public PageReplacement(int pageFrame, int referenceStringLength) {
        this.pageFrame = pageFrame;
        this.referenceStringLength = referenceStringLength;
        this.referenceString = new int[referenceStringLength];
    }

    private void printReferenceString() {
        for (int ref : referenceString) {
            System.out.print(ref + " ");
        }
        System.out.println();
    }

    private void generateReferenceString() {
        Random random = new Random();
        for (int i = 0; i < referenceStringLength; i++) {
            referenceString[i] = random.nextInt(10);
        }
    }

    // private void printPages(Set<Integer> pages) {
    //     System.out.print("Pages: ");
    //     for (int page : pages) {
    //         System.out.print(page + " ");
    //     }
    //     System.out.println();
    // }

    private void fifo() {
        Set<Integer> pages = new HashSet<>();
        Queue<Integer> pageQueue = new LinkedList<>();
        int pageFaults = 0;

        for (int ref : referenceString) {
            if (!pages.contains(ref)) {
                if (pages.size() == pageFrame) {
                    int page = pageQueue.poll();
                    pages.remove(page);
                }
                pageFaults++;
            }
            pages.add(ref);
            pageQueue.add(ref);
        }
        System.out.println("FIFO: " + pageFaults);
    }

    private void lru() {
        Set<Integer> pages = new HashSet<>();
        Queue<Integer> pageQueue = new LinkedList<>();
        int pageFaults = 0;

        for (int ref : referenceString) {
            if (!pages.contains(ref)) {
                if (pages.size() == pageFrame) {
                    int page = pageQueue.poll();
                    pages.remove(page);
                }
                pages.add(ref);
                pageQueue.add(ref);
                pageFaults++;
            } 
            else {
                pageQueue.remove(ref);
                pageQueue.add(ref);
            }
        }
        System.out.println("LRU: " + pageFaults);    
    }

    private void opt() {
        Set<Integer> pages = new HashSet<>();
        int pageFaults = 0;

        for (int refi = 0; refi < referenceStringLength; refi++) {
            if (!pages.contains(referenceString[refi])) {
                if (pages.size() == pageFrame) {
                    int page = -1;
                    int farthest = -1;
                    for (int pageInFrame : pages) {
                        int farthestIndex = 0;
                        for (int i = refi; i < referenceStringLength; i++) {
                            if (referenceString[i] == pageInFrame) {
                                farthestIndex = i;
                                break;
                            }
                        }
                        if (farthestIndex > farthest) {
                            farthest = farthestIndex;
                            page = pageInFrame;
                        }
                    }
                    pages.remove(page);
                }
                pages.add(referenceString[refi]);
                pageFaults++;
            }
        }
        System.out.println("OPT: " + pageFaults);
    }
}
