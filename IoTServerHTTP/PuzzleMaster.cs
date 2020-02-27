using System.Collections.Generic;
using System.IO.Ports;

public class PuzzleMaster {
    public const int NUM_READERS = 4;
    private int nextReaderId = 1; // 1-based sequence so "empty value" of 0 is easy to detect as an error
    private Dictionary<string, int> readerMacs = new Dictionary<string, int>();

    private List<TaggedObject> _readerState;

    static private PuzzleMaster _instance = new PuzzleMaster();

    static PuzzleMaster() { }

    private PuzzleMaster() {
        _readerState = new List<TaggedObject>(NUM_READERS);
        for (int k = 0; k < NUM_READERS; ++k) {
            _readerState.Add(TaggedObject.EMPTY);
        }
    }

    public string AssignObject(string readerMac, TaggedObject obj) {
        // Mac address already assigned to sequence?
        int readerId;
        if (!readerMacs.TryGetValue(readerMac, out readerId)) {
            // Assign next available reader ID
            if (nextReaderId >= 1 && nextReaderId <= NUM_READERS) {
                readerId = nextReaderId++;
                readerMacs.Add(readerMac, readerId);
            }
            else {
                // Error for now
                return null;
            }
        }

        if (readerId < 1 || readerId > NUM_READERS) {
            // Error because we don't recognize this readerId
            return null;
        }
        
        // No solution found, by default
        string flashMode = "0";

        // Update puzzle tracking, look for solution
        _readerState[readerId - 1] = obj;
        PuzzleSolution solution = PuzzleSolution.FindMatchingSolution(_readerState);
        if (solution != null) {
            flashMode = solution.flashMode;

            // DO A THING
            // TEST HACK: Communicate to other parts via serial port
            // NOTE: string[] ports = SerialPort.GetPortNames(); to get port names if needed

            // SerialPort _serialPort = new SerialPort();
            // _serialPort.PortName = "/dev/tty.usbmodem143101";
            // _serialPort.BaudRate = 9600;
            // _serialPort.Open();
            // _serialPort.WriteLine("a"); // "a"mulet activation

            // NOTE: wand first, then amulet, then final ritual
        }
        
        // Return colors for object and a (potentially updated) readerId
        string response = "";
        for (int k = 0; k < 6; ++k) {
            response += ((int)obj.colors[k]).ToString("X6");
        }
        response += flashMode;
        response += readerId.ToString("X2");
        return response;
    }

    static public PuzzleMaster Instance {
        get {
            return _instance;
        }
    }
}