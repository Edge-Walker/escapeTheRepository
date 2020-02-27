using System.Collections.Generic;

public class PuzzleSolution {
    static public PuzzleSolution DRAGON_RITUAL = new PuzzleSolution("1", new[] {
        TaggedObject.DRAGON_ACID, TaggedObject.DRAGON_ACID, TaggedObject.DRAGON_ACID, TaggedObject.DRAGON_ACID
    });
    static public PuzzleSolution BOTTLE_RITUAL = new PuzzleSolution("1", new[] {
        TaggedObject.BOTTLE_MAGENTA, TaggedObject.BOTTLE_BLUE, TaggedObject.BOTTLE_VIOLET, TaggedObject.BOTTLE_ORANGE
    });
    static public PuzzleSolution FINAL_RITUAL = new PuzzleSolution("2", new[] {
        TaggedObject.WIZARD_HAT, TaggedObject.FLOWER, TaggedObject.CARD_5_STARS, TaggedObject.BOTTLE_GREEN
    });

    static private List<PuzzleSolution> _allSolutions;

    private TaggedObject[] _solution;

    public string flashMode { private set; get; }
    private string _radioCommand;

    private PuzzleSolution(string flashMode, TaggedObject[] solution) {
        if (solution.Length != PuzzleMaster.NUM_READERS) {
            throw new System.Exception("Invalid solution length");
        }

        _solution = solution;
        this.flashMode = flashMode;

        if (_allSolutions == null) {
            _allSolutions = new List<PuzzleSolution>();
        }
        _allSolutions.Add(this);
    }

    public bool matches(IEnumerable<TaggedObject> gamestate) {
        // TODO: All COIN_* items are the same
        // TODO: Allow DEBUG_* to be wildcards / even allow for an empty?

        // Look at each rotation of solution
        int offset = 0;
        while (offset < _solution.Length) {
            // Compare this rotation starting at the offset
            int position = offset;
            bool found  = true;
            foreach (var o in gamestate) {
                // Bail the moment we find an object that doesn't match
                if (_solution[position] != o) {
                    found = false;
                    break;
                }
                // Iterate through solution's current rotation, wrapping around
                if (++position >= _solution.Length) {
                    position = 0;
                }
            }
            // Success?
            if (found) {
                return true;
            }
            // Next rotation
            ++offset;
        }
        return false;
    }

    static public PuzzleSolution FindMatchingSolution(IEnumerable<TaggedObject> gamestate) {
        foreach(var o in _allSolutions) {
            if (o.matches(gamestate)) {
                return o;
            }
        }
        return null;
    }
}