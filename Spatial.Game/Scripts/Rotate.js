//<editor-fold desc="import {...} from '@spatial/core';">
const updateComponents = (entity) => (components) => {
    return {
        namespace: "scene",
        type: "update-components",
        payload: {
            entity,
            components,
        },
    };
};

const log = (message) => {
    return {
        namespace: "editor",
        type: "log",
        payload: {
            message,
        },
    };
};

const InputType = Object.freeze({
    NumberRange: 'NumberRange',
    String: 'String',
});
//</editor-fold>

const onUpdateEntity = ({entity, delta, components}) => {
    const {velocity} = components["RotateVelocity"][entity];
    const transform = components["Transform"][entity];
    transform.position.x += velocity * delta;

    return [
        log(`A log message from entity ${entity}`),
        updateComponents(entity)({
            transform
        }),
    ];
};

export default {

    components: {

        RotateVelocity: {
            properties: {
                velocity: {
                    type: InputType.NumberRange,
                    default: 5.0,
                    min: .0,
                    max: 100.0,
                },
            }
        },

        StopRotatingFlag: {
            isFlag: true,
        }

    },

    systems: {

        RotateSystem: {
            query: {
                required: ["Transform", "RotateVelocity"],
                excluded: ["StopRotatingFlag"]
            },
            onUpdateEntity,
        },

    },

}