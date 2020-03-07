using System.Collections.Generic;

public class TaggedObject {
    static public TaggedObject DIAMOND = new TaggedObject("DIAMOND", 87, 43, 140, 245, NeoPixelColor.Aqua, NeoPixelColor.Grey);
    static public TaggedObject CANDY_CANE = new TaggedObject("CANDY_CANE", 23, 180, 138, 245, NeoPixelColor.Red, NeoPixelColor.White);
    static public TaggedObject SKULL = new TaggedObject("SKULL", 119, 180, 138, 245, NeoPixelColor.Brown, NeoPixelColor.Grey);
    static public TaggedObject WIZARD_HAT = new TaggedObject("WIZARD_HAT", 23, 6, 138, 245, NeoPixelColor.Purple, NeoPixelColor.Yellow);
    static public TaggedObject FISH = new TaggedObject("FISH", 247, 42, 140, 245, NeoPixelColor.Orange, NeoPixelColor.Aqua);
    static public TaggedObject GEARS = new TaggedObject("GEARS", 215, 181, 139, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject FLOWER = new TaggedObject("FLOWER", 55, 182, 139, 245, NeoPixelColor.Green, NeoPixelColor.Pink);
    static public TaggedObject MUSHROOM = new TaggedObject("MUSHROOM", 119, 6, 138, 245, NeoPixelColor.Brown, NeoPixelColor.Orange);

    static public TaggedObject CARD_1_CLOCK = new TaggedObject("CARD_1_CLOCK", 23, 181, 139, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Grey, NeoPixelColor.Grey, NeoPixelColor.Grey, NeoPixelColor.Grey);
    static public TaggedObject CARD_2_PENS = new TaggedObject("CARD_2_PENS", 119, 181, 139, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Grey, NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Grey);
    static public TaggedObject CARD_3_CANDLES = new TaggedObject("CARD_3_CANDLES", 135, 42, 140, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Yellow, NeoPixelColor.Grey);
    static public TaggedObject CARD_4_FAERIES = new TaggedObject("CARD_4_FAERIES", 215, 214, 140, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Grey, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Grey);
    static public TaggedObject CARD_5_STARS = new TaggedObject("CARD_5_STARS", 215, 44, 141, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Grey);
    static public TaggedObject CARD_6_BOOKS = new TaggedObject("CARD_6_BOOKS", 55, 215, 140, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow, NeoPixelColor.Yellow);

    static public TaggedObject DRAGON_LIGHTNING = new TaggedObject("DRAGON_LIGHTNING", 162, 87, 26, 39, NeoPixelColor.Yellow, NeoPixelColor.Aqua);
    static public TaggedObject DRAGON_ACID = new TaggedObject("DRAGON_ACID", 208, 8, 205, 37, NeoPixelColor.Yellow, NeoPixelColor.Green);
    static public TaggedObject DRAGON_SHRIEKING = new TaggedObject("DRAGON_SHRIEKING", 192, 200, 16, 37, NeoPixelColor.Grey, NeoPixelColor.Purple);
    static public TaggedObject DRAGON_RUBY = new TaggedObject("DRAGON_RUBY", 113, 161, 84, 27, NeoPixelColor.Red, NeoPixelColor.Brown);
    static public TaggedObject DRAGON_STONE = new TaggedObject("DRAGON_STONE", 192, 198, 58, 37, NeoPixelColor.Grey, NeoPixelColor.White);
    static public TaggedObject DRAGON_WATER = new TaggedObject("DRAGON_WATER", 242, 10, 0, 210, NeoPixelColor.Purple, NeoPixelColor.Aqua);
    static public TaggedObject DRAGON_FIRE = new TaggedObject("DRAGON_FIRE", 246, 37, 23, 37, NeoPixelColor.Red, NeoPixelColor.Yellow);
    static public TaggedObject DRAGON_CRYSTAL = new TaggedObject("DRAGON_CRYSTAL", 208, 153, 194, 37, NeoPixelColor.Blue, NeoPixelColor.Aqua);

    static public TaggedObject GEM_YOUTH = new TaggedObject("GEM_YOUTH", 39, 167, 130, 245, 
        NeoPixelColor.Green, NeoPixelColor.Blue, NeoPixelColor.Purple, NeoPixelColor.Red, NeoPixelColor.Purple, NeoPixelColor.Blue);
    static public TaggedObject GEM_SPRING = new TaggedObject("GEM_SPRING", 55, 221, 131, 245, 
        NeoPixelColor.Blue, NeoPixelColor.Purple, NeoPixelColor.Red, NeoPixelColor.Orange, NeoPixelColor.Red, NeoPixelColor.Purple);
    static public TaggedObject GEM_GOLD = new TaggedObject("GEM_GOLD", 199, 166, 130, 245, 
        NeoPixelColor.Purple, NeoPixelColor.Red, NeoPixelColor.Orange, NeoPixelColor.Yellow, NeoPixelColor.Orange, NeoPixelColor.Red);
    static public TaggedObject GEM_LEAD = new TaggedObject("GEM_LEAD", 71, 223, 129, 245, 
        NeoPixelColor.Red, NeoPixelColor.Orange, NeoPixelColor.Yellow, NeoPixelColor.Green, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject GEM_ADULT = new TaggedObject("GEM_ADULT", 167, 223, 129, 245, 
        NeoPixelColor.Orange, NeoPixelColor.Yellow, NeoPixelColor.Green, NeoPixelColor.Blue, NeoPixelColor.Green, NeoPixelColor.Yellow);
    static public TaggedObject GEM_WINTER = new TaggedObject("GEM_WINTER", 135, 221, 131, 245, 
        NeoPixelColor.Yellow, NeoPixelColor.Green, NeoPixelColor.Blue, NeoPixelColor.Purple, NeoPixelColor.Blue, NeoPixelColor.Green);

    static public TaggedObject BOTTLE_YELLOW = new TaggedObject("BOTTLE_YELLOW", 71, 25, 128, 245, NeoPixelColor.Yellow, NeoPixelColor.Yellow);
    static public TaggedObject BOTTLE_GREEN = new TaggedObject("BOTTLE_GREEN", 151, 24, 128, 245, NeoPixelColor.Green, NeoPixelColor.Green);
    static public TaggedObject BOTTLE_BLUE = new TaggedObject("BOTTLE_BLUE", 39, 19, 128, 245, NeoPixelColor.Blue, NeoPixelColor.Blue);
    static public TaggedObject BOTTLE_MAGENTA = new TaggedObject("BOTTLE_MAGENTA", 71, 190, 128, 245, NeoPixelColor.Pink, NeoPixelColor.Pink);
    static public TaggedObject BOTTLE_RED = new TaggedObject("BOTTLE_RED", 231, 189, 128, 245, NeoPixelColor.Red, NeoPixelColor.Red);
    static public TaggedObject BOTTLE_TEAL = new TaggedObject("BOTTLE_TEAL", 183, 179, 138, 245, NeoPixelColor.Aqua, NeoPixelColor.Aqua);
    static public TaggedObject BOTTLE_ORANGE = new TaggedObject("BOTTLE_ORANGE", 199, 20, 128, 245, NeoPixelColor.Orange, NeoPixelColor.Orange);
    static public TaggedObject BOTTLE_VIOLET = new TaggedObject("BOTTLE_VIOLET", 135, 39, 127, 245, NeoPixelColor.Purple, NeoPixelColor.Purple);

    static public TaggedObject COIN_A = new TaggedObject("COIN_A", 231, 5, 138, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject COIN_B = new TaggedObject("COIN_B", 23, 179, 138, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject COIN_C = new TaggedObject("COIN_C", 39, 39, 127, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject COIN_D = new TaggedObject("COIN_D", 151, 180, 126, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);
    static public TaggedObject COIN_E = new TaggedObject("COIN_E", 119, 179, 138, 245, NeoPixelColor.Yellow, NeoPixelColor.Orange);

    static public TaggedObject COMPASS = new TaggedObject("COMPASS", 39, 193, 135, 245, 
        NeoPixelColor.Red, NeoPixelColor.White, NeoPixelColor.White, NeoPixelColor.Red, NeoPixelColor.White, NeoPixelColor.White);
    static public TaggedObject AMULET = new TaggedObject("AMULET", 71, 218, 136, 245, NeoPixelColor.Brown, NeoPixelColor.Yellow);

    static public TaggedObject DEBUG_A = new TaggedObject("DEBUG_A", 61, 193, 24, 211, 
        NeoPixelColor.Aqua, NeoPixelColor.Pink, NeoPixelColor.White, NeoPixelColor.Pink, NeoPixelColor.Aqua, NeoPixelColor.White);
    static public TaggedObject DEBUG_B = new TaggedObject("DEBUG_B", 86, 147, 203, 131, 
        NeoPixelColor.Red, NeoPixelColor.Orange, NeoPixelColor.Yellow, NeoPixelColor.Green, NeoPixelColor.Blue, NeoPixelColor.Purple);
    static public TaggedObject DEBUG_C = new TaggedObject("DEBUG_C", 245, 143, 22, 211, 
        NeoPixelColor.Pink, NeoPixelColor.Purple, NeoPixelColor.Blue, NeoPixelColor.Blue, NeoPixelColor.Purple, NeoPixelColor.Pink);

    static public TaggedObject EMPTY = new TaggedObject("EMPTY", 0, 0, 0, 0, NeoPixelColor.Black, NeoPixelColor.Black);

    static private List<TaggedObject> allObjects;

    private int[] cardData;

    public NeoPixelColor[] colors {get; private set;}
    public string name{get; private set;}
    public override string ToString() { return name; }

    private TaggedObject(string name, int a, int b, int c, int d, NeoPixelColor color1, NeoPixelColor color2) {
        
        this.name = name;
        cardData = new int[4] { a, b, c, d };
        colors = new NeoPixelColor[6] { color1, color2, color1, color2, color1, color2 };

        if (allObjects == null) {
            allObjects = new List<TaggedObject>();
        }
        allObjects.Add(this);
    }

    private TaggedObject(string name, int a, int b, int c, int d, 
        NeoPixelColor color1, NeoPixelColor color2, NeoPixelColor color3,
        NeoPixelColor color4, NeoPixelColor color5, NeoPixelColor color6) {

        this.name = name;
        cardData = new int[4] { a, b, c, d };
        colors = new NeoPixelColor[6] { color1, color2, color3, color4, color5, color6 };

        if (allObjects == null) {
            allObjects = new List<TaggedObject>();
        }
        allObjects.Add(this);
    }

    public bool matches(CardReaderData card) {
        for (int i = 0; i < 4; ++i) {
            if (card.cardData[i] != cardData[i]) {
                return false;
            }
        }
        return true;
    }

    static public TaggedObject FindObject(CardReaderData card) {
        foreach(var o in allObjects) {
            if (o.matches(card)) {
                return o;
            }
        }
        return null;
    }
}