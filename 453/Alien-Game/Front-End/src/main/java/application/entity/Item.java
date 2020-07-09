package application.entity;

import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

public class Item extends Rectangle {
    public boolean dead = false;
    public final String type;
    public Integer health = 200;
    public Integer player = 0;

    /**
     * Constructor for bullets.
     *
     * @param x     X coordinate
     * @param y     Y coordinate
     * @param w     Width
     * @param h     Height
     * @param type  Type of item
     * @param color Color of item
     */
    public Item(int x, int y, int w, int h, String type, Color color) {
        super(w, h, color);

        this.type = type;
        setTranslateX(x);
        setTranslateY(y);
    }


    /**
     * Constructor for player and aliens.
     *
     * @param x    X coordinate
     * @param y    Y coordinate
     * @param w    Width
     * @param h    Height
     * @param type Type of item
     */
    public Item(int x, int y, int w, int h, String type, Integer player) {
        super(w, h);
        this.player = player;
        this.type = type;
        setTranslateX(x);
        setTranslateY(y);
    }

    public Integer getHealth() {
        return health;
    }


    public Integer getPlayer() {
        return player;
    }

    public void moveLeft() {
        setTranslateX(getTranslateX() - 15);
    }

    public void moveRight() {
        setTranslateX(getTranslateX() + 15);
    }

    public void moveUp() {
        setTranslateY(getTranslateY() - 15);
    }

    public void moveDown() {
        setTranslateY(getTranslateY() + 15);
    }

    public void moveDownBullet() {
        setTranslateY(getTranslateY() + 10);
    }

}