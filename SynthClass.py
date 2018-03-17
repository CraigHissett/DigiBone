import time
import fluidsynth

class TromSynth(object):
    """Detail the outline of the class for reference

    """

    def __init__(self):
        self.fs = fluidsynth.Synth()
        #fs.start()
        ## Your installation of FluidSynth may require a different driver.
        #fs.start(driver="pulseaudio")
        self.fs.start(driver="alsa")

        sfid = self.fs.sfload("example.sf2")
        self.fs.program_select(0, sfid, 0, 0)

    def ChangeSound(self, Sound):
        sfid = self.fs.sfload(Sound)
        self.fs.program_select(0, sfid, 0, 0)

    def PlayExample(self):
        print "PlayExample"
        self.fs.noteon(0, 60, 30)
        self.fs.noteon(0, 67, 30)
        self.fs.noteon(0, 76, 30)

        time.sleep(1.0)

        self.fs.noteoff(0, 60)
        self.fs.noteoff(0, 67)
        self.fs.noteoff(0, 76)

        time.sleep(1.0)

    def CloseSynth(self):
        self.fs.delete()
