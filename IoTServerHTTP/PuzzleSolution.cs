using System.Collections.Generic;

public class PuzzleSolution {
    static public PuzzleSolution DRAGON_RITUAL = new PuzzleSolution("x", "1", new[] {
        TaggedObject.DRAGON_SHRIEKING,
        TaggedObject.DRAGON_CRYSTAL,
        TaggedObject.DRAGON_FIRE,
        TaggedObject.DRAGON_RUBY,
        TaggedObject.DRAGON_LIGHTNING,
        TaggedObject.DRAGON_ACID,
        TaggedObject.DRAGON_STONE,
        TaggedObject.DRAGON_WATER,
    });
    static public PuzzleSolution BOTTLE_RITUAL = new PuzzleSolution("x", "1", new[] {
        TaggedObject.BOTTLE_RED,
        TaggedObject.BOTTLE_BLUE,
        TaggedObject.BOTTLE_GREEN,
        TaggedObject.BOTTLE_ORANGE,
        TaggedObject.BOTTLE_MAGENTA,
        TaggedObject.BOTTLE_TEAL,
        TaggedObject.BOTTLE_VIOLET, 
        TaggedObject.BOTTLE_YELLOW,
    });
    static public PuzzleSolution FINAL_RITUAL = new PuzzleSolution("f", "2", new[] {
        TaggedObject.COIN_A,
        TaggedObject.COMPASS,
        TaggedObject.BOTTLE_RED,
        TaggedObject.GEM_GOLD,
        TaggedObject.WIZARD_HAT,
        TaggedObject.DRAGON_STONE,
        TaggedObject.CARD_5_STARS,
        TaggedObject.AMULET,
    });

    static private List<PuzzleSolution> _allSolutions;

    private TaggedObject[] _solution;

    public string flashMode { private set; get; }
    public string radioCommand { set; get; }

    private PuzzleSolution(string radioCommand, string flashMode, TaggedObject[] solution) {
        if (solution.Length != PuzzleMaster.NUM_READERS) {
            throw new System.Exception("Invalid solution length");
        }

        _solution = solution;
        this.flashMode = flashMode;
        this.radioCommand = radioCommand;

        if (_allSolutions == null) {
            _allSolutions = new List<PuzzleSolution>();
        }
        _allSolutions.Add(this);
    }

    private TaggedObject normalize(TaggedObject o) {
        if (o == TaggedObject.COIN_B ||
            o == TaggedObject.COIN_C ||
            o == TaggedObject.COIN_D ||
            o == TaggedObject.COIN_E) {
            // All coins are the same.
            return TaggedObject.COIN_A;
        }

        if (o == TaggedObject.DEBUG_B ||
            o == TaggedObject.DEBUG_C) {
            // All debug objects are the same.
            return TaggedObject.DEBUG_A;
        }

        return o;
    }

    public bool matches(List<TaggedObject> gamestate) {
        // Look at each rotation of solution
        int offset = 0;
        while (offset < _solution.Length) {
            // Compare this rotation starting at the offset
            int solutionIndex = offset;

            // Find a starting point in gamestate that matches, if any
            int stateOffset = 0;
            bool found = false;
            while (stateOffset < gamestate.Count) {
                if (normalize(gamestate[stateOffset]) == _solution[solutionIndex]) {
                    found = true;
                    break;
                }
                ++stateOffset;
            }
            if (!found) {
                ++offset;
                continue;
            }

            // Now check full rotation for a match
            int stateIndex = stateOffset;
            do {
                var check = normalize(gamestate[stateIndex]);

                // Bail the moment we find an object that doesn't match
                if (_solution[solutionIndex] != check &&
                    // Allow for wildcards / uninitialized spots
                    TaggedObject.DEBUG_A != check) {

                    found = false;
                    break;
                }

                // Iterate through solution's current rotation, wrapping around
                if (++solutionIndex >= _solution.Length) {
                    solutionIndex = 0;
                }
                if (++stateIndex >= gamestate.Count) {
                    stateIndex = 0;
                }
            } while (stateIndex != stateOffset);
            
            // Success?
            if (found) {
                return true;
            }
            // Next rotation
            ++offset;
        }
        return false;
    }

    static public PuzzleSolution FindMatchingSolution(List<TaggedObject> gamestate) {
        foreach(var o in _allSolutions) {
            if (o.matches(gamestate)) {
                return o;
            }
        }
        return null;
    }
}